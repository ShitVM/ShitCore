# ShitCore
ShitVM 핵심 라이브러리

## 요구 사양
아래 사양을 만족하지 않는 시스템에서는 컴파일할 수 없습니다. 
- 1바이트가 8비트인 시스템에서만 작동합니다.
- 포인터의 크기가 최소 32비트 이상인 시스템에서만 작동합니다.
- 널 포인터가 0을 나타내는 시스템에서만 작동합니다.
- 포인터의 크기와 `std::size_t` 타입의 크기가 같은 시스템에서만 작동합니다.

`src/core/Version.cpp` 파일을 수정하면 위 사양을 만족하지 않는 시스템에서도 컴파일 할 수 있습니다. 하지만 ShitVM이 정상적으로 동작하지 않을 수 있습니다.

아래 사양을 만족하지 않는 시스템에서는 정상적으로 동작하지 않습니다.
- 바이트를 Little 또는 Big 엔디안으로, 비트를 Big 엔디안으로 저장하는 시스템에서만 작동합니다.

## 컴파일
```
$ git clone https://github.com/ShitVM/ShitCore.git
$ cd ShitCore
$ cmake .
$ cmake --build .
```

## 주의사항
- ShitCore는 내부 라이브러리입니다. ShitVM 바이트 파일을 생성해야 할 경우 [ShitGen](https://github.com/ShitVM/ShitGen)을 사용하세요.