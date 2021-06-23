# network

# socket


- client 와 server의 연결 유지 
- 채팅, streaming ...
- 두 프로그램간 통신 링크의 엔드포인트
- 포트 넘버를 통해 TCP 레이어가 목적지인 애플리케이션을 식별하게함.
- 엔드포인트: ip + port
- 

![image](https://user-images.githubusercontent.com/43734014/122633471-d864ca80-d113-11eb-8721-0c2230b8cf3a.png)
[그림출처](https://aronglife.tistory.com/entry/NetworkTCPIP-%EC%86%8C%EC%BC%93%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D4%EC%9C%88%EB%8F%84%EC%9A%B0-%EC%84%9C%EB%B2%84-%EA%B5%AC%ED%98%84?category=910808)



# http (응용계층)


- client가 보내는 요청에 server가 응답 
- 응답처리후 연결 종료
- 단방향적 통신
- 비연결성 프로토콜 -> 요청/응답 방식
- 도메인 + URL, 도메인 + URI 를 통해서 요청, HTML을 통한 응답
- Cookie 와 Session 이용

[참고](https://velog.io/@doomchit_3/Internet-HTTP-%EA%B0%9C%EB%85%90%EC%B0%A8%EB%A0%B7-IMBETPY)

# Blocking / Non-Blocking

## Blocking
- I/O 작업이 진행되는 동안 자신의 작업을 중단한채 대기

1. 유저는 커널에게 read 작업 요청
2. 데이터가 입력될 때까지 대기
3. 데이터가 입력되면 유저에게 커널이 결과를 전달한다.


## Non-Blocking
- I/O 작업이 진행되는 동안 유저 프로세스의 작업을 중단시키지 않는 방식
- 반복적인 시스템 호출(폴링 방식과 비슷)

1. 유저가 커널에게 read 작업 요청
2. 데이터가 입력됐든 안됬는 요청하는 그 순간, 바로 결과가 반환된다. (데이터가 없다면 없다는 결과 메시지 EWOULDBLOCK) 반환
3. 입력 데이터가 있다면 1-2번 반복. (2번에서 결과 메시지를 받은 유저는 다른 작업을 진행한다.)
4. 입력 데이터가 있다면 커널이 유저에게 결과 전달한다.

# Synchronouse(동기) / Asynchronous(비동기)
이벤트 통지 모델

## Synchronouse(동기)
- I/O 결과를 기다렸다가 이벤트를 직접 처리

## Asynchronous(비동기)
- I/O 작업이 진행되는 동안 결과를 기다리지 않고, 이벤트 핸들러를 통해 알림이 오면 처리


![image](https://user-images.githubusercontent.com/43734014/123053734-298bfb80-d43f-11eb-8b28-8b0bcd7a7f3c.png)
[참고](https://junshock5.tistory.com/148)
