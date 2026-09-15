### 구현했지만 수정이 필요한 부분
++, --
switch문의 조건검사.
함수별 스택 할당 시, 해당 함수에 필요한 만큼의 크기만 할당하기. 현재는 모든 변수 크기만큼을 각각의 함수에 할당하고 있음.

큰 크기의 정수값 이용할 때, imm 크기를 벗어나는 경우에 대한 처리필요. (컴파일러와 어셈블러 모두 수정 필요.) - 완.

parser의 라벨 문법 수정 필요
(https://claude.ai/chat/336a622c-88d3-42cc-a72c-701aebeeeea0)
(https://claude.ai/chat/2dcc7579-412d-4993-8edf-8cd10c79f4e9)

ASM에 추가할 명령어: SIGNEXT/ZEROEXT/BYTECUT 삼종세트, 고정소수점 나눗셈.

"다만 별개로 후위 증가의 결과값 보존 문제는 있습니다. 현재 출력은 b를 증가시킨 뒤 다시 읽어서 (int)b++의 결과로 저장하므로, 원래 값 7 대신 8을 사용합니다. 지금 예제는 그 결과를 버려서 영향이 없지만, int x = b++;에서는 드러납니다. [후위 증가 처리 (line 489)](//wsl.localhost/Ubuntu-24.04/home/slash/gisa_compiler/gisa_tag_generator.c:489)에서 증가 전 값을 별도 임시변수에 보존해야 합니다."


taggen의 casting, temp 심볼테이블 등록 부분 코드 정리 필요. - 완


### 메모

brother가 있다면 컨테이너로 감싸라.


### 감상
chapter_8/valid/empty_loop_body.c와 chapter_9/valid/stack_arguments/test_for_memory_leaks.c는 ISS에서 테스트 불가. 몇억번의 명령어 수행이 필요한 테스트케이스라 ISS로는 너무 오랜 시간이 걸린다.(대충 따져봐도 며칠은 돌려야 할듯.) 하지만 이런 테스트도 onboard에 올려 돌려보니 대략 몇십초만에 완료된다!

tag 생성기에서, 조건문/반복문에서의 점프 명령이 break/continue 두 개다 보니까, tac의 rA/rB를 받는 두 개의 입력 구조에 정확히 맞아 떨어진다! 만들고 보니 깊이가 얼마나 되던 rA rB break continue를 주고받고 끌어올리는게 딱딱 맞물리게 들어가는게 재밌구만.



### 구현이 필요한 별도의 기능
간단한 링커
여러 파일 지원(일단은 그냥 파일 각각 컴파일 후 수동으로 해도 될듯)



### 다음으로 할 순서
fpga cpu에 uart 합치기 -> 완!
uart와 cpu 활용해 printf scanf 역할의 함수 만들기 -> 완!
cpu에서 돌아가는 간단한 프로그램 만들기!
 - 계산기 -> 완!
 - 선택지 텍겜
 - 초간단 탑뷰 rpg: 맵 있고 주인공 이동하다가 아이템 얻기/적 만나면 jrpg 전투하기!
교재 10장 만들기 -> 완!
 fpga cpu에서 나눗셈 만들기 -> 완!




보낼 메일
컴파일러: llvm은 프론트 백 구분되어 머신 독립적이라고 배웠는데, 구현하며 찾아보니 llvm ir에서는 타입 사이즈를 정해지게 만든다고 함. 이럼 머신에 맞는 백엔드를 만들어도 해당 ir을 백엔드에 넣으면 잘못된 컴파일이 되는게 아닌지.

남병규 교수님: LISA의 pc relative ldr/str의 존재이유. lisa는 imem과 dmem이 분리된 구조인데, 현재 pc값을 dmem의 주소를 찾는데 어떻게 사용하는 의미가 있는건지. imem은 연결구조상 ldr/str로 접근이 불가능하고.



KW_INT/KW_LONG 등 타입 키워드의 token_value로 사이즈롤 표현하는 방식?
타입을 바이트폭 사이즈로 정규화는 tag에? codegen에? 생각해보기.