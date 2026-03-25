# Verilog로 구현한 세 종류의 Prefix Tree Adder 비교
구현한 Adder_Subtractor은 Kogge-Stone tree, Brent-Kung tree, Han-Carlson tree이다. 모두 32비트이며, input cell, black cell, carry cell, sum cell의 조합으로 만들었다.

 - Kogge-Stone Adder는 모든 비트 위치에서 black cell 연산을 수행하는 방식이다.
단계 수가 log₂N 로 복잡도가 낮다는 장점을 가졌지만, black cell이 밀집되고 많은 와이어 배선으로 인해 면적 및 전력 소모가 크다는 단점이 있다.

 - Brent-Kung Adder는 홀수 위치의 비트에서 먼저 black cell 연산을 수행한 후, 추가 단계를 거쳐 짝수 위치로 전파해 결과를 구하는 방식이다.
각 row에서, 한 쌍의 column마다 최대 하나의 cell만 가지기 때문에 게이트의 수가 적고 와이어의 폭증이 없다는 장점이 있지만, 단계 수가 2log₂N - 1 로 많아 복잡도가 높다는 단점이 있다.

 - Han-Carlson Adder는 Kogge-Stone과 Brent-Kung을 혼합한 덧셈기로, 홀수 비트에서는 K-S를, 짝수 비트에는 B-K를 사용했다.
각 row에서, 한 쌍의 column마다 최대 하나의 cell만 가져 게이트가 적다는 B-K의 장점을 취하고, K-S 스타일을 받아서 배선 길이를 늘리는 대신 단계 수를 log₂N + 1 로 줄인다. 

---

## Kogge-Stone Adder_Subtractor
#### Post-Mapping Map Viewer Result

<img width="800" alt="Image" src="https://github.com/user-attachments/assets/9f44b68d-5d97-4fe0-b174-e48e4fe43f0d" />

#### Used ALMs

<img width="507" height="323" alt="Image" src="https://github.com/user-attachments/assets/4db32d9c-4770-4167-a79f-aaf322594788" />

#### Critical Path

<img width="370" height="131" alt="Image" src="https://github.com/user-attachments/assets/fa080c58-66be-4d3e-b539-a345c9fae37b" />

---

## Brent-Kung Adder_Subtractor
#### Post-Mapping Map Viewer Result

<img width="800" alt="Image" src="https://github.com/user-attachments/assets/6741e589-b725-44dd-a5ba-6281e3f612bd" />

#### Used ALMs

<img width="507" height="323" alt="Image" src="https://github.com/user-attachments/assets/13d72554-5c26-4040-9a84-4de5177b126e" />

#### Critical Path

<img width="370" height="131" alt="Image" src="https://github.com/user-attachments/assets/8500649c-d8ad-4885-9674-76335b221412" />

---

## Han-Carlson Adder_Subtractor
#### Post-Mapping Map Viewer Result

<img width="800" alt="Image" src="https://github.com/user-attachments/assets/777a8c5c-0fa5-4553-b678-39bd1d9a83e7" />

#### Used ALMs

<img width="507" height="323" alt="Image" src="https://github.com/user-attachments/assets/4b70fb32-95f3-4fe8-9741-a8ca1491d722" />

#### Critical Path

<img width="370" height="131" alt="Image" src="https://github.com/user-attachments/assets/c6826bfb-1952-4334-a736-988d6483c24c" />

---

## 결론

| 종류 | ALM 사용량 | 딜레이 |
|---|---|---|
| Kogge-Stone | 226 | 12.212 |
| Brent-Kung | 154 | 13.165 |
| Han-Carlson | 177 | 12.752 |


구현 결과들을 비교하자, 이론과 동일하게 Kogge-Stone이 가장 많은 ALM 사용량과 가장 짧은 딜레이를, Brent-Kung은 가장 적은 ALM 사용량과 가장 긴 딜레이를, Han-Carlson은 그 사이의 ALM 사용량과 딜레이를 가지는 것을 볼 수 있다.

Han-Carlson는 Kogge-Stone보다 49개 적은 ALM과 0.540 느린 딜레이를, Brent-Kung보다 23개 많은 ALM과 0.413 빠른 딜레이를 보인다.


따라서 두 수치 모두 고려했을 때 Han-Carlson Adder가 최적의 선택지라고 판단하여 채택하였다.
