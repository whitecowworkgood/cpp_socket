# cpp_socket
c++를 이용하여 소켓프로그래밍 공부

1. 기본적인 클래스를 이용하여 소켓 틀을 잡아 놓음

2. 파일을 읽어서 전송하는 기능 구현
2.1 파일입출력 o
2.2 바이너리 형식의 파일은 오류가 발생 -> 이를 해결해야 함.
2.3 해시함수 사용으로 무결성 검사하기 -> 해시값을 계산까지는끝냈으나, 소켓 전송시 완전히 보내지지 않고 일부분만 보내짐, 이를 좀더 고민해 봐야함
2.4 구조체 형식으로 전송 -> 해시값과 데이터를 같이 보내는 방안 생각해 보기

3. 쓰레드 구현 o
