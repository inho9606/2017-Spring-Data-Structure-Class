// 기사의 여행, DFS 알고리즘 사용
#include <stdio.h>
#include <stdlib.h>
#pragma warning  (disable:4996)
#include <time.h>
int Npos(int currenti, int currentj, int mode); // 현재 위치에서 이동 가능한 경로 찾는 함수
void kmove(int i, int j, int count); // 나이트를 이동시키는 함수
void decide_next(int *currenti, int *currentj, int nn); // Npos에서 찾은 경로 중 실제로 이동할 위치를 결정하는 함수
int board[8][8] = { { -1,-1,-1,-1,-1,-1,-1,-1 },{ -1,-1,-1,-1,-1,-1,-1,-1 },{ -1,-1,-1,-1,-1,-1,-1,-1 },{ -1,-1,-1,-1,-1,-1,-1,-1 },{ -1,-1,-1,-1,-1,-1,-1,-1 },{ -1,-1,-1,-1,-1,-1,-1,-1 },{ -1,-1,-1,-1,-1,-1,-1,-1 },{ -1,-1,-1,-1,-1,-1,-1,-1 } }; // 체스판을 전역변수로 선언
int next_square[8][2]; // 현재 위치에서 이동 가능한 square의 좌표를 저장하는 배열
int main()
{
	int i, j, m = 0, npos; // i와 j는 나이트가 위치하는 좌표에 이용, m은 나이트가 움직이는 횟수 카운트, npos는 현재 위치에서 이동 가능한 경우의 수를 저장
	double duration; // 런타임 시간을 저장함
	while (1) // 제대로 된 입력을 받을 때까지 무한루프 실행
	{
		printf("체스판 위 나이트의 시작 좌표를 양식에 맞게 입력해 주세요. 가로 세로 좌표는 각각 0~7 사이의 값이어야 합니다.\n");
		printf("입력 양식 예: 0, 0\n");
		scanf("%d, %d", &i, &j); // 양식에 맞게 입력받은 좌표를 i와 j에 각각 저장함
		if (0 <= i && i <= 7 && 0 <= j && j <= 7) break; // i와 j가 체스판 위의 정상 범위에 있으면 반복문 중단
		else printf("잘못 입력하셨습니다.\n"); // 체스판 위에 존재하지 않는 좌표를 입력한 경우 입력을 다시 받음
	}
	clock_t start = clock(); // 현재 시간 저장
	kmove(i, j, m); // 사용자로부터 입력받은 좌표로 나이트를 이동시킴
	m += 1; // 체스판에 나이트가 한 번 올라갔으므로 나이트의 이동 횟수를 의미하는 m을 1 증가시킴.
	while (1 <= m && m <= 63) // 나이트의 이동을 최대 63번 반복함. 체스판은 총 64칸이 존재하기 때문
	{
		npos = Npos(i, j, 0); // 현재 위치에서 이동 가능한 경로의 수를 조사하고 이동 가능한 좌표를 next_square 배열에 저장함
		if (npos == 0) // 현재 위치에서 이동이 불가능한 경우
		{
			printf("The end!\n");
			break; // 반복문 탈출 및 프로그램 종료.
		}
		else if (npos == 1) // 이동 가능한 수가 한 개인 경우
		{
			i = next_square[0][0]; 
			// next_square 배열의 가장 첫 번째 좌표값을 i, j가 가짐. Npos 함수가 이동 가능한 경우의 수를 반환함과 동시에 next_square 배열 앞부터 이동 가능한 좌표를 저장하기 때문에 가능
			j = next_square[0][1];
			kmove(i, j, m); // 체스판에서 나이트를 i, j 좌표로 이동함
		}
		else // 현재 위치에서 이동 가능한 수가 1개 초과일 경우
		{
			decide_next(&i, &j, npos); // 2개 이상의 이동 가능한 좌표 중 실제로 이동할 좌표를 결정하여 포인터로 i, j를 그 값으로 변경함
			kmove(i, j, m); // 변경된 i, j의 좌표로 나이트를 이동함
		}
		m += 1; // 나이트가 한 번 이동했다는 의미
	}
	duration = ((double)(clock() - start)) / (double)CLK_TCK; // 나이트가 이동하는 데 걸린 시간을 구함
	printf("\n실행시간: %f초 ", duration);
	system("pause");
	return 0;
}

int Npos(int currenti, int currentj, int mode) // 현재 위치에서 이동 가능한 경우의 수를 반환하는 함수로, 파라미터로 현재좌표(currenti, currentj)와 mode를 가짐
{
	int ktmove[8][2] = { { -2,1 },{ -1,2 },{ 1,2 },{ 2,1 },{ 2,-1 },{ 1,-2 },{ -1,-2 },{ -2,-1 } }; // 나이트가 이동할 수 있는 경로를 좌표로 나타낸 것
	int x, n = 0; // x는 for문을 통해 ktmove의 값을 읽고, n은 이동 가능한 경우의 수를 저장함
	for (x = 0; x<8; x++) // ktmove의 배열 크기가 8이어서 x가 8까지 도는것
	{
		if (0 <= ktmove[x][0] + currenti && ktmove[x][0] + currenti <= 7 && 0 <= ktmove[x][1] + currentj && ktmove[x][1] + currentj <= 7) 
			// ktmove 안에 있는 나이트의 이동 가능 경로와 현재 나이트가 위치한 좌표를 더해서 나이트가 이동했을 때의 좌표가 체스판 위에 존재하는 square이라면 
		{
			if (board[ktmove[x][0] + currenti][ktmove[x][1] + currentj] == -1) // 나이트가 이동할 square를 나이트가 이전에 한 번도 방문한 적이 없는 경우
			{
				if (mode == 0) // 그리고 mode가 0이면
				{
					next_square[n][0] = ktmove[x][0] + currenti; // next_square 배열에 유효한 이동 가능 좌표를 저장함
					next_square[n][1] = ktmove[x][1] + currentj;
					n += 1; // 현재 위치에서 이동 가능한 경우의 수를 1 증가시킴
				}
				else n += 1; // 모드가 0이 아닌 경우, next_square 배열에 좌표를 저장하지 않고 이동 가능 경우의 수만 1 증가시킴
			}
		}
	}
	return n; // 함수를 호출한 좌표로부터 유효한 이동 가능 횟수를 반환함
}

void kmove(int i, int j, int count) // 체스판 위 i, j 좌표에 나이트가 count번째로 지나갔다는 표시를 하는 함수
{
	board[i][j] = count; // 나이트가 count번째로 이동했다는 표시로 해당 좌표에 count을 저장함.
	printf("(%d, %d): %d ", i, j, count); // 나이트가 이동해 온 좌표와 그 칸이 몇 번째로 방문한 칸인지를 출력함
}

void decide_next(int *currenti, int *currentj, int nn) 
{// 이동 가능한 경우의 수가 여러개인 경우, 여러 개의 선택지 중 유효한 하나의 경우를 결정하는 함수로, 현재 위치 좌표를 포인터로 받고 함수를 호출한 지점에서의 이동 가능 횟수를 nn에 저장함
	int x, minimum = 9; 
	// x는 이동 가능한 횟수의 범위 nn에서 유효한 이동 가능 좌표 next_square의 값을 읽고, minimum은 next_square의 좌표값으로부터 이동 가능한 횟수 중 가장 작은 값을 찾아 저장함
	int nexti, nextj; // 실제로 이동하게 될 좌표를 저장하는 변수 nexti, nextj
	for (x = 0; x<nn; x++)
	{
		if (Npos(next_square[x][0], next_square[x][1], 1) < minimum) 
 // next_square 좌표로부터 유효한 이동 가능 경우의 수가 minimum보다 작은 경우(여기서 Npos 함수를 모드 1로 호출하기 때문에 next_square의 값들은 바뀌지 않고 이동 가능한 숫자만 반환된다)
		{
			nexti = next_square[x][0];
			nextj = next_square[x][1]; // nexti nextj에 조건을 만족하는 next_square 의 좌표를 저장한다.
			minimum = Npos(nexti, nextj, 1); // 저장한 next_square의 좌표로부터 유효한 이동 가능 횟수를 minimum에 저장한다.
		}
	} // 위의 반복이 끝나면 nexti와 nextj에는 next_square 좌표 중에서 유효한 이동 가능 횟수를 가장 작게 가진 좌표가 저장되게 된다.
	*currenti = nexti;
	*currentj = nextj; // 메인 함수의 i, j를 가리키고 있는 포인터를 활용해서 메인의 i, j 값을 nexti, nextj로 바꿔준다.
}
