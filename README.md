08/08 초기 커밋, 윈도우 창을 우선적으로 띄우는것을 목표로하였음. 

1.Initialize함수를 호출하고 InitMainWindow로 윈도우창 초기화를 시도함

2.WNDCLASSEX타입으로 새로 만들 윈도우 창의 속성을 정의해주고 RegisterClassEx로 등록을해준다.

3.해상도 설정을 위해 RECT 타입을 이용,AdjustWindowRect함수를 사용하는 이유는 해상도 계산을 위의 드래그 창을 제외하고 계산하기 위해서임

4.가장 중요한 CreateWindow 함수로 윈도우를 만들어줌, ShowWindow로 띄우고 UpdateWindow는  WM_PAINT를 즉시보내서 한번 업데이트 시켜줌.  WM_PAINT는 운영체제가 창이 변하면 알아서 전달해줘서 초기화 
이후에 따로 주지 않아도됨

5.Run함수를 보면 콜백되는 메세지가 WM_QUIT가 아니면 무한 루프되는구조로, 창을 유지시킴
