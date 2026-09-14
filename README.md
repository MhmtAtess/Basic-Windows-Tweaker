# PremiumOverlay (WinTweaker) 🚀

[![C++20](https://img.shields.io/badge/Language-C%2B%2B20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![DirectX 11](https://img.shields.io/badge/Graphics-DirectX%2011-green.svg)](https://docs.microsoft.com/en-us/windows/win32/direct3d11/at-a-glance)
[![ImGui](https://img.shields.io/badge/GUI-Dear%20ImGui-orange.svg)](https://github.com/ocornut/imgui)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%2F%2011-0078D6.svg)](https://www.microsoft.com/windows)

[English](#english) | [Türkçe](#türkçe)

---

<a name="english"></a>
## 🇬🇧 English

### Overview
**PremiumOverlay** (WinTweaker) is a modern, high-performance Windows optimization and tweaking application featuring a hardware-accelerated DirectX 11 borderless UI overlay powered by Dear ImGui. It combines system performance tuning, latency reduction, bloatware removal, UI customizations, and automated system maintenance into a single fluid desktop tool.

### Key Features
- **Real-Time System Monitoring**: Live FPS meter, RAM load monitoring, and detailed hardware specification detector (CPU, GPU, RAM, OS, Disk).
- **Performance & Power Tuning**: Unlock Windows Ultimate Performance power plan, disable telemetry services (`DiagTrack`), and apply CPU priority responsiveness tweaks.
- **Gaming & Latency Optimization**: Reduce system timer resolution down to **0.5ms**, apply 1:1 raw mouse input fixes, disable Nagle's algorithm for lower network latency, force Windows Game Mode, and clear GPU shader caches (Nvidia, AMD, DirectX).
- **Privacy & System Control**: Pause forced Windows updates, disable Start Menu & Lock Screen advertisements, stop location tracking, and manage Windows Defender real-time protection.
- **Windows UI & Explorer Tweaks**: Restore Windows 11 classic right-click context menu, add 'Take Ownership' shortcut, toggle hidden files/extensions, and align taskbar positions.
- **Debloat & Software Installer**: Batch remove UWP bloatware apps and seamlessly install essential desktop applications via WinGet.
- **System Cleaner & Maintenance**: Clean `%TEMP%`, Prefetch, and Windows Update cache files; trigger background `SFC /scannow`, DISM image repair, and SSD/HDD TRIM defragmentation.
- **Modern UI & Themes**: Custom particle background renderer, smooth cubic animation system, theme switching (Dark, Midnight, Neon), custom accent colors, and VSync toggles.

### Project Architecture
```
PremiumOverlay/
├── CMakeLists.txt              # CMake build definitions & FetchContent setup
└── src/
    ├── main.cpp                # Application entry point (WinMain)
    ├── components/
    │   └── Controls.cpp/.h     # Custom animated buttons, toggles & cards
    ├── core/
    │   ├── Application.cpp/.h  # Win32 borderless window & message loop
    │   ├── ConfigManager.h     # Persistent JSON settings manager
    │   ├── HardwareInfo.cpp/.h # WMI / DXGI / Registry hardware detection
    │   └── Logger.h            # Thread-safe in-app operation logger
    ├── effects/
    │   └── ParticleSystem.cpp/.h # DX11 background interactive particle canvas
    ├── pages/                  # Modular UI tab view implementations
    ├── renderer/
    │   └── DX11Renderer.cpp/.h # DirectX 11 device, swapchain & render targets
    ├── tweaks/                 # System Registry & Win32 API execution modules
    └── ui/
        ├── AnimationManager.h  # Lerp & easing animation engine
        ├── ThemeManager.cpp/.h # Color palettes & styling
        └── UIManager.cpp/.h    # ImGui frame orchestration & layout
```

### Prerequisites & Dependencies
- **OS**: Windows 10 / Windows 11 (64-bit)
- **Compiler**: MSVC v143+ (Visual Studio 2022 recommended) with C++20 support
- **Build System**: CMake 3.24 or higher
- **Dependencies** (automatically pulled via CMake FetchContent):
  - [Dear ImGui](https://github.com/ocornut/imgui) (Docking branch)
  - [nlohmann/json](https://github.com/nlohmann/json) (v3.11.2)

### Building from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/PremiumOverlay.git
   cd PremiumOverlay
   ```

2. Generate project files with CMake:
   ```bash
   cmake -B build -S .
   ```

3. Build the project:
   ```bash
   cmake --build build --config Release
   ```
   *The compiled executable `WinTweaker.exe` will be located in `build/bin/Release/`.*

---

<a name="türkçe"></a>
## 🇹🇷 Türkçe

### Genel Bakış
**PremiumOverlay** (WinTweaker), Dear ImGui ve donanım hızlandırmalı DirectX 11 altyapısıyla geliştirilmiş, modern ve yüksek performanslı bir Windows ince ayar (tweaker) ve sistem optimizasyon uygulamasıdır. Sistem performansını artırma, gecikme (latency) düşürme, gereksiz uygulamaları (bloatware) kaldırma, arayüz kişiselleştirme ve sistem bakımını şık bir masaüstü arayüzünde bir araya getirir.

### Öne Çıkan Özellikler
- **Gerçek Zamanlı Sistem İzleme**: Canlı FPS göstergesi, RAM yükü takibi ve detaylı donanım bilgisi tespiti (İşlemci, Ekran Kartı, Bellek, İşletim Sistemi, Disk).
- **Performans ve Güç İnce Ayarları**: Windows Gizli "Nihai Performans" güç planını etkinleştirme, telemetri hizmetlerini (`DiagTrack`) kapatma ve CPU öncelik optimizasyonları.
- **Oyun ve Gecikme (Latency) İyileştirmeleri**: Sistem zamanlayıcı hassasiyetini **0.5ms** değerine düşürme, 1:1 ham fare girdisi (raw input) ayarlama, ağ paket gecikmelerini önleyen Nagle algoritmasını kapatma, Oyun Modunu zorlama ve GPU shader önbelleklerini (Nvidia, AMD, DirectX) temizleme.
- **Gizlilik ve Sistem Kontrolü**: Otomatik Windows güncellemelerini duraklatma, Başlat menüsü ve Kilit ekranı reklamlarını kaldırma, konum takibini engelleme ve Windows Defender gerçek zamanlı korumasını yönetme.
- **Windows Arayüz & Gezgin Özelleştirmeleri**: Windows 11 klasik sağ tık menüsünü geri getirme, 'Sahipliği Al' kısayolu ekleme, gizli dosya/uzantı görünürlüğü ve görev çubuğu hizalama.
- **Bloatware Kaldırıcı & Yazılım Yükleyici**: Dahili UWP bloatware uygulamalarını topluca kaldırma ve WinGet altyapısı ile temel masaüstü yazılımlarını otomatik yükleme.
- **Sistem Temizleyici ve Bakım**: `%TEMP%`, Prefetch ve Windows Update önbelleklerini temizleme; arka planda `SFC /scannow`, DISM imaj onarımı ve SSD/HDD TRIM optimizasyonlarını çalıştırma.
- **Modern Arayüz ve Temalar**: Özel parçacık (particle) arka plan animasyonu, pürüzsüz geçiş efektleri, tema seçenekleri (Karanlık, Gece Yarısı, Neon), özel vurgu renkleri ve VSync kontrolü.

### Proje Mimarisi
```
PremiumOverlay/
├── CMakeLists.txt              # CMake yapılandırması ve otomatik bağımlılık yönetimi
└── src/
    ├── main.cpp                # Uygulama giriş noktası (WinMain)
    ├── components/
    │   └── Controls.cpp/.h     # Özel animasyonlu butonlar, anahtarlar ve kartlar
    ├── core/
    │   ├── Application.cpp/.h  # Çerçevesiz Win32 penceresi ve mesaj döngüsü
    │   ├── ConfigManager.h     # Kalıcı JSON ayar yöneticisi
    │   ├── HardwareInfo.cpp/.h # WMI / DXGI / Registry donanım tespiti
    │   └── Logger.h            # İşlem günlüğü (log) tutucu
    ├── effects/
    │   └── ParticleSystem.cpp/.h # DX11 tabanlı hareketli parçacık tuvali
    ├── pages/                  # Modüler sekme görünümleri
    ├── renderer/
    │   └── DX11Renderer.cpp/.h # DirectX 11 aygıtı ve render yönetimi
    ├── tweaks/                 # Windows Kayıt Defteri ve API müdahale modülleri
    └── ui/
        ├── AnimationManager.h  # Animasyon ve yumuşatma motoru
        ├── ThemeManager.cpp/.h # Renk paletleri ve stiller
        └── UIManager.cpp/.h    # ImGui düzeni ve sayfa yönetimi
```

### Gereksinimler ve Derleme

1. Depoyu klonlayın:
   ```bash
   git clone https://github.com/kullanici-adiniz/PremiumOverlay.git
   cd PremiumOverlay
   ```

2. CMake ile proje dosyalarını oluşturun:
   ```bash
   cmake -B build -S .
   ```

3. Uygulamayı derleyin:
   ```bash
   cmake --build build --config Release
   ```
   *Derlenen `WinTweaker.exe` dosyası `build/bin/Release/` dizininde yer alacaktır.*

---

### License / Lisans
Distributed under the MIT License. See `LICENSE` for more information.
