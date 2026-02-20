# TinyTensor-C
TinyML için C dilinde bellek verimli dinamik tensör uygulaması. Kısıtlı kaynaklara sahip mikrodenetleyiciler için Union tabanlı yapılar kullanarak Float32 ve Int8 kuantizasyonunu destekler.
Markdown# TinyTensor: Embedded Dynamic Tensor Library

TinyTensor, kısıtlı kaynaklara (RAM/Flash) sahip mikrodenetleyiciler için tasarlanmış, C dilinde yazılmış hafif ve dinamik bir tensör kütüphanesidir. Bu proje, TinyML uygulamalarında bellek kullanımını optimize etmek amacıyla geliştirilmiştir.

## 🚀 Temel Özellikler

- **Dinamik Tip Yönetimi**: Aynı bellek bloğu üzerinde `Float32` ve `Int8` (Quantized) verileri saklayabilme.
- **Bellek Verimliliği**: C dilindeki `union` yapısı kullanılarak RAM ayak izi %75'e kadar azaltılmıştır.
- **Lineer Quantization**: Float verileri düşük kayıpla 8-bit tam sayılara dönüştürme algoritması.
- **Embedded Friendly**: Bağımlılık gerektirmeyen (sadece standart C kütüphaneleri), taşınabilir kod yapısı.

## 🛠 Teknik Mimari

Proje, bellek yönetimini optimize etmek için aşağıdaki yapıyı kullanır:

```c
typedef struct {
    union {
        float* f32_ptr;
        int8_t* i8_ptr;
    } data;
    TensorType type;
    float scale;
    int8_t zero_point;
} TinyTensor;
Quantization FormülüGerçek değerleri tamsayıya dönüştürmek için kullanılan standart TinyML formülü:$$r = S(q - Z)$$Burada:r: Gerçek değer (Float)S: Scale (Ölçek)q: Quantized değer (Int8)Z: Zero-point (Sıfır noktası)💻 Kurulum ve ÇalıştırmaGereksinimlerHerhangi bir C derleyicisi (GCC, Clang, MSVC vb.)CMake (isteğe bağlı)DerlemeBashgcc dinamiktensor.c -o tinytensor_demo
ÇalıştırmaBash./tinytensor_demo
📊 Demo AnaliziUygulama çalıştırıldığında aşağıdaki adımları gerçekleştirir:5 elemanlı bir float dizisi oluşturur.Bu verileri dinamik aralığına göre int8 formatına sıkıştırır.Bellek adreslerini (pointer) kontrol ederek union yapısının doğruluğunu teyit eder.Veriyi tekrar dequantize ederek hata payını (accuracy loss) hesaplar.
