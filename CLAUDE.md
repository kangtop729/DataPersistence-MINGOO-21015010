# CLAUDE.md

이 문서는 Claude(에이전트)가 이 저장소에서 작업할 때 따라야 할 규칙을 정리한다.

## 프로젝트 개요

**JSON 파일 기반 데이터 영속성**(저장/불러오기)과 CRUD를 검증하는 PoC(Proof of Concept)다. Main
프로젝트인 [`SampleOrderSystem-MINGOO-21015010`](https://github.com/kangtop729/SampleOrderSystem-MINGOO-21015010)의
"JSON 파일 영속성" 기술 검증용 참고 산출물이며, 반도체 시료 도메인 로직은 다루지 않는다.
세부 계획은 [`PLAN.md`](PLAN.md) 참고.

## 기술 스택

- 언어: C++20
- 빌드: Visual Studio / MSBuild (`.vcxproj`)
- 플랫폼: x64 / Debug, Release
- 테스트: GoogleTest / GoogleMock (`gmock` 1.11.0, NuGet 패키지)
- JSON: nlohmann/json 3.11.3 (single header, `src/ThirdParty/nlohmann/json.hpp`에 vendoring — NuGet
  패키지가 아니므로 git에 그대로 커밋되어 있음)

## 프로젝트 구성

[`ConsoleMVC-MINGOO-21015010`](https://github.com/kangtop729/ConsoleMVC-MINGOO-21015010)과 동일하게,
별도 테스트 프로젝트/Configuration 없이 `DataPersistence-MINGOO-21015010.vcxproj`의 기본 Debug/Release
2개 Configuration만 사용한다.

- `src/main.cpp`는 두 Configuration 모두에서 공통으로 빌드되는 유일한 진입점이다. `_DEBUG` 전처리기
  정의(Debug Configuration에서 기본 설정됨) 여부로 동작이 갈린다.
  - `Debug`: `main()`이 `::testing::InitGoogleTest` + `RUN_ALL_TESTS()`를 호출해 테스트를 실행한다.
    `test/*.cpp`와 gmock/gtest 소스(`gmock.targets` import)가 Debug에서만 함께 빌드된다.
  - `Release`: CRUD 데모 콘솔 앱을 실행한다. `test/*.cpp`는 빌드에서 제외된다.
- Model/Repository/Serialization의 `.cpp`는 두 Configuration 모두에서 공통으로 빌드된다.

## 빌드 및 테스트 명령

```
# 테스트 빌드 및 실행 (Debug 빌드 = 테스트 실행)
msbuild DataPersistence-MINGOO-21015010.vcxproj /p:Configuration=Debug /p:Platform=x64
x64\Debug\DataPersistence-MINGOO-21015010.exe

# CRUD 데모 앱 빌드/실행
msbuild DataPersistence-MINGOO-21015010.vcxproj /p:Configuration=Release /p:Platform=x64
x64\Release\DataPersistence-MINGOO-21015010.exe
```

새 테스트 파일을 추가할 때는 `.vcxproj`의 `test\*.cpp` `<ClCompile>` 항목에 등록해야 하며, Release에서
`ExcludedFromBuild` 처리되어 있는지 확인한다.

## 작업 원칙

1. **인터페이스 분리**: `IRepository<T>` 인터페이스만으로 CRUD가 가능해야 하며, JSON 세부 구현(파일
   경로, 파싱 방식)이 호출자에게 노출되지 않아야 한다.
2. **영속성 방식 고정**: 반드시 JSON 파일 기반으로 구현한다 (Main 프로젝트와 일치해야 하는 항목).
3. **가벼운 TDD**: 이 저장소는 PoC이므로 Main만큼 촘촘한 경계값 테스트를 요구하지 않는다. 핵심
   시나리오 1~2개(예: 저장 후 재로드 시 데이터 일치)에 대해 실패하는 테스트를 먼저 작성하고 확인한
   뒤, 최소 구현으로 통과시키는 Red-Green 사이클을 따른다. 실제 파일 시스템에 저장/재로드하는 통합
   테스트를 최소 1개 포함한다.
4. **작은 단위 커밋 + 자동 push**: Phase 단위 혹은 그보다 작은, 의미 있는 단위로 커밋한다. 커밋 후
   바로 push까지 수행하고, 사용자는 push된 결과를 보고 검토한다 (커밋 전에 멈춰서 검토를 기다리지
   않는다).
5. **커밋 메시지 규칙**: 제목은 반드시 아래 5개 prefix 중 하나로 시작한다.
   - `[feature]` 새 기능 추가
   - `[refactor]` 동작 변경 없는 구조 개선
   - `[fix]` 버그 수정
   - `[docs]` 문서 변경
   - `[test]` 테스트 추가/수정, 테스트 인프라 변경
6. **Clean Code**: 함수는 한 가지 일만 하도록 작게 유지하고, 매직 넘버 대신 의미 있는 이름의 상수를
   사용하며, 불필요한 주석 대신 좋은 이름으로 의도를 드러낸다. 다만 PoC 특성상 Main만큼 세세한
   리팩토링/리뷰까지는 요구하지 않는다.
7. **불필요한 추상화 금지**: 명세에 없는 기능(동시성 제어, 여러 저장 백엔드 등)을 미리 만들지 않는다.
