# 2023_NOsquito
2023.03.02. ~ 2023.06.14.
![image](https://github.com/user-attachments/assets/a2c3fb6a-e320-4e36-89d8-8d9186ff0c82)
![ezgif-7-b44a1ab584](https://github.com/user-attachments/assets/638c7825-c2d3-4d36-9e69-f53bb4a1f538)

| 파일명                     | 용도                                              |
|----------------------------|---------------------------------------------------|
| **[```main_nosquito.c```](main_nosquito.c)** | main, 타이머의 인터럽트를 통한 일정 주기 ADC 실행       |
| **[```nosquito_adc_fft.h```](Include/nosquito_adc_fft.h)** | ADC로 읽은 파형을 푸리에 변환, 출력하는 함수      |
| **[```nosquito_bluetooth_serial.h```](Include/nosquito_bluetooth_serial.h)** | USART를 통한 블루투스 사용, 스마트폰과 통신        |
| **[```nosquito_display.h```](Include/nosquito_display.h)** | TFT_LCD 화면 출력 함수                           |
| **[```nosquito_images.h```](Include/nosquito_images.h)** | TFT_LCD 화면 이미지 문자열                       |
| **[```nosquito_led.h```](Include/nosquito_led.h)** | RGB LED strip 선택 함수                          |
| **[```nosquito_motor_pwm.h```](Include/nosquito_motor_pwm.h)** | 타이머1을 통한 PWM 출력 및 서보모터 선택 함수     |

## 작동 과정
0. Flowchart<br>
![image01](https://github.com/user-attachments/assets/b4f239b7-6da6-4d99-9fdb-c1d18bbce7c6)

1. 전원을 켜면 초기 화면을 출력한다.<br>
![image](https://github.com/user-attachments/assets/ef608e09-694c-47f0-af81-a13e52951542)

2. 사용하고자 하는 레지스터 할당을 마치면 마이크 센서를 통해 주변 소리를 감지하여 푸리에 변환 알고리즘을 통해 주파수를 감지한다.<br>
디스플레이에는 두 마이크의 푸리에 변환 결과 그래프를 함께 표시한다.<br>
![image](https://github.com/user-attachments/assets/525d8781-3018-453f-941f-fc2f9629ef1f)

3. 암컷 모기의 날개짓 소리를 감지하게 되면 디스플레이에 화면을 출력하고 추적 알고리즘을 통해 모기의 위치를 식별한다.<br>
![image](https://github.com/user-attachments/assets/2ef2674e-19f9-435e-949d-be5bdd52ffed)
  
    - 위치를 비교함으로 점수를 부여하고 최종적인 위치를 결정한다.<br>
    - 감지의 선택지는 총 4가지이며 동시에 스마트폰에 알람 메시지를 전송한다.<br>
![image](https://github.com/user-attachments/assets/6b0b4efa-f991-4165-a5d8-083da37bbc04)
![image](https://github.com/user-attachments/assets/9a9ac20c-c7a3-496f-bad1-8f30f0d7bc76)<br>
![image](https://github.com/user-attachments/assets/f7030939-6ed2-42b0-a07a-2a4ffc945edf)
![image](https://github.com/user-attachments/assets/4128f1b1-4e16-4e19-a7b6-2a2a41e08985)

4. 방향이 결정되면 해당 방향에 위치한 LED에 불이 들어오고<br>
![image](https://github.com/user-attachments/assets/8a041907-d5c3-49d0-8523-455fe02359c3)
![image](https://github.com/user-attachments/assets/1748f2db-c74c-4003-b615-f558d9094e9d)

5. 방향에 따른 모터의 동작으로 인해 스프레이가 분사된다.<br>
![image](https://github.com/user-attachments/assets/065db39d-075a-480d-89bb-bd86d057a76c)
![image](https://github.com/user-attachments/assets/f2ca44fb-3ca2-40cb-bb71-ab75c14018a8)

6. 스프레이 분사가 완료되면 스프레이가 분사되었다는 알람을 스마트폰에 전송한 후 다시 동작 1의 상태로 돌아간다.<br>
![image](https://github.com/user-attachments/assets/19e8a207-8687-4cf8-91a1-5d0e9b152295)

## HW
### HW 작동 모습
- 푸리에 변환 결과 그래프 출력 모습<br>
![ezgif-7-09722a0b7b](https://github.com/user-attachments/assets/effe27c2-aa8c-4b59-8280-aa174b9a8c12)

- LED 스트립 색깔 변화<br>
![image](https://github.com/user-attachments/assets/16b61afb-89ad-4678-ab1f-fb998388a035)
![image](https://github.com/user-attachments/assets/da4df843-76e7-411b-9207-c9b8185c6a06)

- 서보모터 작동(스프레이)<br>
![ezgif-7-9092cdb2de](https://github.com/user-attachments/assets/d652bfc0-c8c7-4e7c-aa3d-477cc4bf6c7b)

### HW 설계 세부 모습
- Schematics<br>
![NOsquito230611_2_1](https://github.com/user-attachments/assets/5e766fb8-d9cc-480b-af2d-f2e30c5d79e3)

- 쉴드 (만능기판)<br>
![image](https://github.com/user-attachments/assets/d7d88f98-c234-498c-b199-f9071868ea1c)
![20230611_205637](https://github.com/user-attachments/assets/7babc10f-7acf-4aaf-9ce6-b2f03f670898)

- DEMUX 및 풀다운 저항<br>
![image](https://github.com/user-attachments/assets/925e2492-43e5-479e-9ff5-22c116afdc60)

- 마이크 증폭 모듈<br>
![image](https://github.com/user-attachments/assets/dfb7eb01-e7e4-450a-8581-b2b04b138cf5)

- 스프레이 분사기 구조 3D 형상 모델링 설계<br>
![image](https://github.com/user-attachments/assets/3abfa441-88fa-46f6-ba4c-6d02c62d3fbe)
![image](https://github.com/user-attachments/assets/99fbfea3-a717-4edb-b256-52a19a296a65)<br>
![image](https://github.com/user-attachments/assets/bdb996f8-8899-431d-8b2b-6965af5147cf)
![image](https://github.com/user-attachments/assets/2c23f569-9421-4ad4-af5c-bef35df45543)

## 기능
1) 모기의 소리, 특정 데시벨을 센서로 감지하여 모기를 식별할 수 있다.
2) 모기약(스프레이)을 분사할 수 있는 모터를 설정하고 원하는 방향에 사용할 수 있다.
3) 블루투스 기술을 활용하여 휴대폰에 정보를 전달하고 만든 앱을 통해 확인할 수 있다.
4) 센서를 활용해 모기를 감지하면 위치에 따라 LED 소자를 turn-on/off 할 수 있다. 
5) TFT-LCD를 활용해 모기의 위치와 정보를 시각화한다.
6) 타이머 인터럽트를 활용해 실시간으로 모기를 식별하며 디스플레이에 위치를 표시한다.

## 연구 범위
1. **주파수 센싱:** 마이크로폰 센서로 측정되는 음성의 파형을 분석하기 위해 ```FFT(Fast Fourier Transform)``` 알고리즘을 사용하여 소프트웨어적으로 ```400Hz(암컷 모기의 날개짓 주파수)```를 기준으로 모기를 감지한다.
2. **경고 시스템:** 모기 감지 직후 TFT LCD 디스플레이에 모기 사진을 표시하고, GPIO 제어를 통해 평소 흰색 조명으로 사용되던 LED는 빨간색으로 색깔이 바뀌어 초기 경보를 알린다. 추가적으로 블루투스 기능을 활용하여 스마트폰에 경고문구를 전송하고 경보음을 울리게 한다.
3. **모기 위치 표시:** 모기 인식 직후 양방향으로 배치된 마이크로폰 센서를 통해 모기가 있을 확률이 높은 방향을 파악하고, TFT LCD 디스플레이에 모기의 위치를 표시한다.(1. 왼쪽 감지 2. 오른쪽 감지 3. 양방향 감지 4. 모기 감지 실패)
4. **4방향 스프레이 분사 시스템:** 모기가 있을 확률이 높은 벽 모서리에 위치한 스프레이를 분사한다.
5. **스프레이 최대 분사 횟수 제한:** 공기중의 모기약 성분이 충분히 있을 경우 더 이상 분사할 필요가 없는 시점이 발생하므로, 적절하게 분사 횟수를 조절한다.
6. **모기 감지 시점 기록:** 스마트폰으로 동시에 마지막 모기 감지 시점을 블루투스 모듈을 통해 전송하여 기록한다.
7. **스마트폰 경보:** 스마트폰으로 동시에 알림을 전송하여 사용자가 모기의 존재를 인식하도록 돕는다.

### 역할 분배
| [```KimTeddy(팀장)```](https://github.com/KimTeddy/) | ```팀원1``` | ```팀원2``` | 연구내용                                   |
|--------|--------|--------|----------------------------------------|
| O      | O      | O      | 플로우차트에 따른 프로그램 알고리즘 구현        |
| O      | O      | O      | 발표자료 제작                              |
| O      |        | O      | 필요 부품 구매                           |
| O      |        | O      | LED 조명 경보 시스템 구현                 |
| O      |        |        | 스프레이 분사 장치 3D 모델링 및 프린팅       |
| O      |        |        | 스프레이 분사 장치 서보모터 제어           |
| O      |        |        | 회로도 제작                              |
|        | O      |        | 앱인벤터를 활용한 앱 구현(스마트폰 블루투스 연결) |
|        | O      |        | 모기 위치 추적 알고리즘                  |
|        |        | O      | 원룸 형태 모형 제작                      |

### 간트 차트
| 구분   | 추진내용                          | 3월  | 4월  | 5월  | 6월  |
|--------|-----------------------------------|------|------|------|------|
| 계획   | 자료조사                          |<-----|----->|      |      |
|        | H/W 및 S/W 설계 계획              |      |<---->|      |      |
| 분석   | 제품 필요성 분석                  |<-----|----->|      |      |
| 설계   | 제품 외형 설계                    |      |<-----|------|----->|
|        | 회로도 제작                       |      |      |      |<---->|
| 개발   | 소자 납땜                        |      |<----|----->|      |
|        | 센서 신호 처리                    |      |      |<---->|      |
|        | 플로우차트에 따른 알고리즘 프로그래밍 |      |<-----|----->|      |
| 테스트 | S/W에 따른 제품 작동 테스트        |      |<-----|----->|      |
| 종료   | 프로젝트 종료                     |      |      |      |<---->|

### 소요 개발비
| 항목     | 용도                 | 부품명                       | 소요비용(원)          |
|----------|----------------------|------------------------------|-------------------|
| 전자 부품 | 음성 센싱            | 마이크 증폭 모듈             | 0 (기존 보유)    |
|          | 모기 위치 표시       | 5V RGB LED Strip             | 6,050           |
|       | 3채널 벽면 LED,<br>4채널 서보모터 핀확장| demultiplexer                | 4,000           |
|          | 스프레이 분사         | 서보모터                     | 0 (기존 보유)    |
|          | 스마트폰과 통신       | 블루투스 모듈                | 0 (기존 보유)    |
|          | 이미지 출력, 위치 표시 | TFT LCD(키트)                | 0 (기존 보유)    |
| 일반 재료 | 모기약 스프레이       | 스프레이                     | 0 (기존 보유)    |
|          | 원룸 모형            | 우드락                       | 2,000           |
|          | 사실적인 원룸 모형    | 시트지, 벽지                 | 1,580           |
| **총계** |                      |                              | **13,630**      |
