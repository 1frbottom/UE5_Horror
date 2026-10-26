# Horror

> UE5 싱글 플레이 공포 게임

<p align="center">
  <img src="https://github.com/user-attachments/assets/bd2bd75f-d24c-4fd5-a075-8399f6eb7300" width="600" />
</p>
<p align="center">
  <img src="https://github.com/user-attachments/assets/3469f01b-d25a-4738-abb6-9f46c3661995" width="295" />
  <img src="https://github.com/user-attachments/assets/cb1401e9-499a-44c7-892e-1d381c105fea" width="295" />
</p>

* 유튜브 ([Preview](https://youtu.be/4RwZF5kehHY?si=qTP8RrAY4gl4cQl8))
* 실행파일 ([Google Drive](https://drive.google.com/drive/folders/1u5auzDjyY_MjojGrmEuv4grukbGNAZRq?usp=sharing))
* 기술문서 ([PDF](https://github.com/user-attachments/files/22330252/5_.pdf))

<br>

## 프로젝트 개요

* **개발 기간 :** 2025.03 ~ 2025.09
* **개발 인원 :** 2명 (디자인 1 / 프로그래밍 1)
* **엔진 및 언어 :** Unreal Engine 5.4 / C++ / Blueprint
* **플랫폼 :** PC (Windows)
* **배경 :** 픽션인 SCP의 SCP-1983을 재구성한 배경으로, 집 안에서 시작합니다. 1층의 특정 잠긴 문을 열어 탈출하면 1스테이지가 클리어됩니다.

<br>

## 핵심 구현

| 항목 | 구현 | 코드 / 위치 |
| :--- | :---: | :--- |
| 고스트 AI 컨트롤러 (BT / BB 실행) | C++ | [HRAIController](UE5_HorrorGame/Source/UE5_HorrorGame/AI/HRAIController.cpp#L28-L58) |
| 고스트 공격 Task / 공격 판정 | C++ | [BTTask_Attack](UE5_HorrorGame/Source/UE5_HorrorGame/AI/BTTask_Attack.cpp#L15-L44), [AttackHitCheck](UE5_HorrorGame/Source/UE5_HorrorGame/Character/HRCharacterNonPlayer.cpp#L242-L268) |
| Sight / Hearing 감지, 행동 흐름 구성 | Blueprint / BTree | `Content/Blueprint/Ghost/` 의 `BP_AIController`, `BT_GhostCharacter_Perception` |
| 시점 전환 (1인칭 / 숄더 / 쿼터) | C++ | [SetCharacterControl](UE5_HorrorGame/Source/UE5_HorrorGame/Character/HRCharacterPlayer.cpp#L591-L649) |
| 상호작용 실행 (인터페이스) | C++ + Blueprint | [Interact](UE5_HorrorGame/Source/UE5_HorrorGame/Character/HRCharacterPlayer.cpp#L1093-L1099), [포커스 처리](UE5_HorrorGame/Source/UE5_HorrorGame/InteractableActor/HRInteractableActorBase.cpp#L70-L144) |

<br>

## 현재 구현 범위

현재 1스테이지까지 구현되어 있습니다.

| 항목 | 내용 |
| :--- | :--- |
| 게임 상태 | 타이틀, 인게임(일시정지), 종료(사망 / 클리어) |
| 조작 | 걷기, 달리기(스태미나), 숙이기 |
| 상호작용 | 아이템, 서랍, 문, 트리거 |
| 고스트 | 순찰, 감지(Sight / Hearing), 추적, 공격 |

<br>

## 조작 방법

| 키 | 동작 |
| :--- | :--- |
| `W` `A` `S` `D` | 이동 |
| `Left Ctrl` | 숙이기 |
| `Left Shift` | 달리기 |
| `F` | 상호작용 |
| `M` | 지도 보기 (획득 시) |
| `ESC` | 일시정지 |

<br>

## 실행 방법

위 Google Drive 링크에서 파일을 받아 압축을 해제한 뒤 `exe`를 실행합니다.

<br>