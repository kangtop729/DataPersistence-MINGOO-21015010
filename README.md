# DataPersistence-MINGOO-21015010

JSON 파일 기반 데이터 영속성(저장/불러오기)과 CRUD를 검증하는 PoC.

Main 프로젝트 [`SampleOrderSystem-MINGOO-21015010`](https://github.com/kangtop729/SampleOrderSystem-MINGOO-21015010)의
"JSON 파일 영속성" 기술 요소를 검증하기 위한 참고용 산출물이다.

## 목적

- 프로세스를 재시작해도 데이터가 유지되는 JSON 파일 기반 영속성을 검증한다.
- `IRepository<T>` 인터페이스만으로 CRUD가 가능하고, JSON 세부 구현이 노출되지 않는 구조를 실험한다.

## 구조

```
DataPersistence-MINGOO-21015010/
├── src/
│   ├── Model/                       # 저장 대상 엔티티 (Item)
│   ├── Repository/                  # IRepository<T> + JsonFileRepository
│   ├── Serialization/               # to_json/from_json 변환
│   ├── ThirdParty/nlohmann/json.hpp # nlohmann/json single header
│   └── main.cpp                     # CRUD 데모 실행
├── test/                            # gmock 기반 단위/통합 테스트 (Debug 빌드에 포함)
├── CLAUDE.md                        # 에이전트 작업 규칙
├── PLAN.md                          # 구현 계획
└── README.md
```

자세한 구현 계획은 [`PLAN.md`](PLAN.md) 참고.

## 빌드 및 실행

[`ConsoleMVC-MINGOO-21015010`](https://github.com/kangtop729/ConsoleMVC-MINGOO-21015010)과 동일하게,
별도 테스트 프로젝트 없이 하나의 `.vcxproj`에서 Debug/Release 두 Configuration만 사용한다.
Debug로 빌드하면 실행 파일 자체가 gmock 테스트 러너가 되고, Release로 빌드해야 실제 CRUD 데모 앱이 뜬다.

```
# 테스트 빌드 및 실행 (Debug 빌드 = 테스트 실행)
msbuild DataPersistence-MINGOO-21015010.vcxproj /p:Configuration=Debug /p:Platform=x64
x64\Debug\DataPersistence-MINGOO-21015010.exe

# CRUD 데모 앱 빌드/실행
msbuild DataPersistence-MINGOO-21015010.vcxproj /p:Configuration=Release /p:Platform=x64
x64\Release\DataPersistence-MINGOO-21015010.exe
```

(구현 진행에 따라 테스트 구성/실행 결과 절 추가 예정)
