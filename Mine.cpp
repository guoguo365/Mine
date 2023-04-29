#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>

#define ROW 10
#define COL 10
#define IMGW 40

void initMap(int map[ROW][COL]);

void showMap(int map[ROW][COL]);

void drawMap(int map[ROW][COL], IMAGE img[]);

void mouseEvent(int map[ROW][COL]);

void openEmpty(int map[ROW][COL], int row, int col);

void judge(int map[ROW][COL], int row, int col);

int getHitGridNum(int map[ROW][COL]);

bool isOver = false;

int main() {

	// ����ͼ�δ���
	initgraph(COL * IMGW, ROW * IMGW, SHOWCONSOLE);

	// �������������
	srand((unsigned)time(NULL));

	// ������Ϸ��ͼ
	int map[ROW][COL] = { 0 };

	// ��ʼ������
	initMap(map);

	// ����ͼƬ
	IMAGE img[12];

	for (int i = 0; i < 12; i++) {
		char fileName[50] = { 0 };
		sprintf_s(fileName, "./images/%d.png", i);
		loadimage(img + i, fileName, IMGW, IMGW);
		putimage(i * IMGW, 0, img + i);
	}

	while (true) {

		mouseEvent(map);

		// ��ͼ
		drawMap(map, img);

		if (isOver) {
			int ret = MessageBox(GetHWnd(), "�����ˣ��Ƿ�����һ�Σ�", "hint", MB_OKCANCEL);
			if (ret == IDOK) {
				initMap(map);
				isOver = false;
			}
			else {
				exit(999);
			}
		}
	}

	system("pause");
	return 0;
}

void showMap(int map[ROW][COL]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%2d ", map[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void initMap(int map[ROW][COL]) {

	// map����
	memset(map, 0, ROW * COL * sizeof(int));

	// ����ͼ�������10���ף�����-1��ʾ
	for (int i = 0; i < 10;) {
		int r = rand() % ROW;
		int c = rand() % COL;

		// �����ȥ��
		if (map[r][c] == 0) {
			map[r][c] = -1;
			i++;
		}
	}

	// ������
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == -1) {

				// ���������ڵľŹ���������+1
				for (int r = i - 1; r <= i + 1; r++) {
					for (int c = j - 1; c <= j + 1; c++) {

						// ��������Խ��
						if ((r >= 0 && r < ROW && c >= 0 && c < COL) && map[r][c] != -1) {

							// ����+1
							map[r][c] ++;
						}
					}
				}
			}
		}
	}

	// �����еĸ��Ӷ�������ͼ����������ÿ�����Զ����ܣ����綼����20
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			map[i][j] += 20;
		}
	}
}

void drawMap(int map[ROW][COL], IMAGE img[]) {

	// ����ӳ���ͼ��
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == -1) {

				// ��Ⱦ����
				putimage(j * IMGW, i * IMGW, img + 9);
			}
			else if (map[i][j] >= 0 && map[i][j] <= 8) {

				// ��Ⱦ����
				putimage(j * IMGW, i * IMGW, img + map[i][j]);
			}
			else if (map[i][j] > 18 && map[i][j] < 29) {

				// ��Ⱦ����
				putimage(j * IMGW, i * IMGW, img + 10);
			}
			else if (map[i][j] >= 29) {

				// ��Ⱦ����
				putimage(j * IMGW, i * IMGW, img + 11);
			}
		}
	}

}

void mouseEvent(int map[ROW][COL]) {

	// ������Ϣ�ṹ��
	ExMessage msg;
	if (peekmessage(&msg, EM_MOUSE)) {

		// ���������ת����������±�
		int r = msg.y / IMGW;
		int c = msg.x / IMGW;

		// ����������
		if (msg.message == WM_LBUTTONDOWN) {
			if (map[r][c] > 18 && map[r][c] < 29) {
				map[r][c] -= 20;
				openEmpty(map, r, c);
				judge(map, r, c);
				showMap(map);
			}
		}

		// ����Ҽ����£�����
		else if (msg.message == WM_RBUTTONDOWN) {
			if (map[r][c] > 18 && map[r][c] < 29) {
				map[r][c] += 30;
			}
			else if (map[r][c] >= 29) {
				map[r][c] -= 30;
			}
			showMap(map);
		}
	}
}

void openEmpty(int map[ROW][COL], int row, int col) {

	// �ж��Լ��Ƿ�Ϊ��
	if (map[row][col] == 0) {

		// �ж����ڵľŹ����Ƿ�Ϊ��
		for (int r = row - 1; r <= row + 1; r++) {
			for (int c = col - 1; c <= col + 1; c++) {

				// �ж��Ƿ�Խ���Ϊ��
				if ((r >= 0 && r < ROW && c >= 0 && c < COL) && map[r][c] - 20 == 0) {

					// ��
					map[r][c] -= 20;
					openEmpty(map, r, c);
				}
			}
		}

	}
}

void judge(int map[ROW][COL], int row, int col) {

	// �ж��Ƿ�㵽��
	if (map[row][col] == -1) {

		// ������ʾ�����е���
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if (map[i][j] - 20 == -1) {
					map[i][j] -= 20;
				}
			}
		}
		isOver = true;
	}

	// �ж��Ƿ�ͨ��
	if (getHitGridNum(map) >= ROW * COL - 10) {
		int ret = MessageBox(GetHWnd(), "��ϲ���أ�����\n\n�Ƿ�����һ�֣�", "hint", MB_OKCANCEL);
		if (ret == IDOK) {
			initMap(map);
			isOver = false;
		}
		else {
			exit(100);
		}
	}

}

int getHitGridNum(int map[ROW][COL]) {
	int ret = 0;

	// ������ȡ�㿪�ĸ�����
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] < 19) {
				ret++;
			}
		}
	}
	return ret;
}