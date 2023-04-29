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

	// 创建图形窗口
	initgraph(COL * IMGW, ROW * IMGW, SHOWCONSOLE);

	// 设置随机数种子
	srand((unsigned)time(NULL));

	// 定义游戏地图
	int map[ROW][COL] = { 0 };

	// 初始化数据
	initMap(map);

	// 加载图片
	IMAGE img[12];

	for (int i = 0; i < 12; i++) {
		char fileName[50] = { 0 };
		sprintf_s(fileName, "./images/%d.png", i);
		loadimage(img + i, fileName, IMGW, IMGW);
		putimage(i * IMGW, 0, img + i);
	}

	while (true) {

		mouseEvent(map);

		// 绘图
		drawMap(map, img);

		if (isOver) {
			int ret = MessageBox(GetHWnd(), "你死了，是否重来一次？", "hint", MB_OKCANCEL);
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

	// map清零
	memset(map, 0, ROW * COL * sizeof(int));

	// 给地图里随机埋10个雷，雷用-1表示
	for (int i = 0; i < 10;) {
		int r = rand() % ROW;
		int c = rand() % COL;

		// 随机数去重
		if (map[r][c] == 0) {
			map[r][c] = -1;
			i++;
		}
	}

	// 遍历雷
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == -1) {

				// 遍历雷所在的九宫格，雷数量+1
				for (int r = i - 1; r <= i + 1; r++) {
					for (int c = j - 1; c <= j + 1; c++) {

						// 处理数组越界
						if ((r >= 0 && r < ROW && c >= 0 && c < COL) && map[r][c] != -1) {

							// 雷数+1
							map[r][c] ++;
						}
					}
				}
			}
		}
	}

	// 把所有的格子都用遮罩图盖起来，给每个各自都加密，比如都加上20
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			map[i][j] += 20;
		}
	}
}

void drawMap(int map[ROW][COL], IMAGE img[]) {

	// 数据映射成图形
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == -1) {

				// 渲染地雷
				putimage(j * IMGW, i * IMGW, img + 9);
			}
			else if (map[i][j] >= 0 && map[i][j] <= 8) {

				// 渲染数字
				putimage(j * IMGW, i * IMGW, img + map[i][j]);
			}
			else if (map[i][j] > 18 && map[i][j] < 29) {

				// 渲染遮罩
				putimage(j * IMGW, i * IMGW, img + 10);
			}
			else if (map[i][j] >= 29) {

				// 渲染旗子
				putimage(j * IMGW, i * IMGW, img + 11);
			}
		}
	}

}

void mouseEvent(int map[ROW][COL]) {

	// 定义消息结构体
	ExMessage msg;
	if (peekmessage(&msg, EM_MOUSE)) {

		// 把鼠标坐标转换成数组的下标
		int r = msg.y / IMGW;
		int c = msg.x / IMGW;

		// 鼠标左键按下
		if (msg.message == WM_LBUTTONDOWN) {
			if (map[r][c] > 18 && map[r][c] < 29) {
				map[r][c] -= 20;
				openEmpty(map, r, c);
				judge(map, r, c);
				showMap(map);
			}
		}

		// 鼠标右键按下，插旗
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

	// 判断自己是否为空
	if (map[row][col] == 0) {

		// 判断相邻的九宫格是否为空
		for (int r = row - 1; r <= row + 1; r++) {
			for (int c = col - 1; c <= col + 1; c++) {

				// 判断是否越界和为空
				if ((r >= 0 && r < ROW && c >= 0 && c < COL) && map[r][c] - 20 == 0) {

					// 打开
					map[r][c] -= 20;
					openEmpty(map, r, c);
				}
			}
		}

	}
}

void judge(int map[ROW][COL], int row, int col) {

	// 判断是否点到雷
	if (map[row][col] == -1) {

		// 遍历显示出所有的雷
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if (map[i][j] - 20 == -1) {
					map[i][j] -= 20;
				}
			}
		}
		isOver = true;
	}

	// 判断是否通关
	if (getHitGridNum(map) >= ROW * COL - 10) {
		int ret = MessageBox(GetHWnd(), "恭喜过关！！！\n\n是否再来一局？", "hint", MB_OKCANCEL);
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

	// 遍历获取点开的格子数
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] < 19) {
				ret++;
			}
		}
	}
	return ret;
}