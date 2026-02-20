#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * @brief Tensor Veri Tipleri
 */
typedef enum {
    TYPE_F32 = 0,   // 32-bit Float (Flout)
    TYPE_I8  = 1    // 8-bit Quantized Integer (İnt-sekiz)
} TensorType;

/**
 * @brief Dinamik Tensör Yapısı
 * Union (Yun-yın) kullanarak bellek tasarrufu sağlıyoruz.
 */
typedef struct {
    union {
        float* f32_ptr;    // Float erişimi
        int8_t* i8_ptr;    // Quantized (Kuvantize) erişimi
    } data;
    
    TensorType type;       
    size_t length;         
    float scale;           // Sıkıştırma katsayısı (Skeyl)
    int8_t zero_point;     // Sıfır noktası kayması (Ziro poynt)
} TinyTensor;

/**
 * @brief Yeni bir tensör nesnesi oluşturur
 */
TinyTensor* create_tensor(size_t length, TensorType type) {
    TinyTensor* t = (TinyTensor*)malloc(sizeof(TinyTensor));
    if (t == NULL) return NULL; // Bellek kontrolü

    t->length = length;
    t->type = type;
    
    // Bellek tahsisi (En büyük tip olan float üzerinden yer açıyoruz)
    t->data.f32_ptr = (float*)malloc(length * sizeof(float));
    return t;
}

/**
 * @brief Float veriyi 8-bit Integer'a sıkıştırır (Quantization)
 */
void quantize_f32_to_i8(TinyTensor* t, float* raw_input) {
    float min_val = raw_input[0];
    float max_val = raw_input[0];
    size_t i; // Değişkeni fonksiyonun başında bir kez tanımladık

    // 1. Adım: Dinamik aralığı bul (Min-Max)
    for (i = 0; i < t->length; i++) {
        if (raw_input[i] < min_val) min_val = raw_input[i];
        if (raw_input[i] > max_val) max_val = raw_input[i];
    }

    // 2. Adım: Scale ve Zero Point hesapla
    t->scale = (max_val - min_val) / 255.0f;
    
    // Scale sıfır olmamalı (bölme hatası için kontrol)
    if(t->scale == 0) t->scale = 1.0f;

    t->zero_point = (int8_t)(-min_val / t->scale - 128);

    // 3. Adım: Veriyi dönüştür ve belleği 'int8' olarak kullan
    for (i = 0; i < t->length; i++) {
        t->data.i8_ptr[i] = (int8_t)(raw_input[i] / t->scale + t->zero_point);
    }
    
    t->type = TYPE_I8; 
}

/**
 * @brief Sıkıştırılmış veriyi tekrar Float'a çevirir (Dequantization)
 */
float dequantize(TinyTensor* t, size_t index) {
    return t->scale * (t->data.i8_ptr[index] - t->zero_point);
}

/**
 * @brief Belleği serbest bırak (Memory Leak - Memori Liik önleme)
 */
void free_tensor(TinyTensor* t) {
    if (t != NULL) {
        if (t->data.f32_ptr != NULL) free(t->data.f32_ptr);
        free(t);
    }
}

// --- Örnek Uygulama (Demo) ---
int main() {
    size_t i; // Döngü için i tanımlandı
    float sensor_data[5] = {12.5f, 45.2f, -10.0f, 32.8f, 0.5f};
    
    printf("--- TinyML Dinamik Tensor Uygulamasi ---\n\n");
    
    TinyTensor* my_tensor = create_tensor(5, TYPE_F32);

    printf("[ADIM 1] Quantization (Kuvantizasyon) islemi basliyor...\n");
    quantize_f32_to_i8(my_tensor, sensor_data);

    printf("Scale: %f | Zero Point: %d\n\n", my_tensor->scale, my_tensor->zero_point);

    printf("[ADIM 2] Bellek Analizi:\n");
    printf("Union adresi (f32): %p\n", (void*)my_tensor->data.f32_ptr);
    printf("Union adresi (i8):  %p\n", (void*)my_tensor->data.i8_ptr);
    printf("Ayni adreste farkli tipler saklaniyor!\n\n");

    printf("[ADIM 3] Dogruluk Testi:\n");
    for (i = 0; i < 5; i++) {
        float recovered = dequantize(my_tensor, i);
        float error = sensor_data[i] - recovered;
        printf("Orijinal: %6.2f | Geri Donen: %6.2f | Hata: %6.4f\n", 
                sensor_data[i], recovered, error);
    }

    free_tensor(my_tensor);
    return 0;
}
