#include <graphics.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define BOARD_SIZE 21

enum class Player { BLACK_PLAYER, WHITE_PLAYER, NONE };

double const GRID_SIZE = SCREEN_HEIGHT / (BOARD_SIZE - 1);
double const GRID_LEFT = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;

COLORREF BACKGROUND = 0x99eeee;

Player board[BOARD_SIZE][BOARD_SIZE];// ����

class Button
{
public:
    Button(int left, int top, int width, int height, LPCTSTR text);
    void Draw();
    bool isClick(EASYXMSG* msg);

private:
    int left = 0;
    int top = 0;
    int width = 0;
    int height = 0;
    LPCTSTR text = _T("");
};

Button::Button(int left, int top, int width, int height, LPCTSTR text)
{
    this->left = left;
    this->top = top;
    this->width = width;
    this->height = height;
    this->text = text;
}

void Button::Draw()
{
    setfillcolor(BLUE);
    fillrectangle(left, top, left + width, top + height);
    settextcolor(WHITE);
    setbkcolor(BLUE);
    outtextxy(left + 10, top + 15, text);
}

bool Button::isClick(EASYXMSG* msg)
{
    if (msg->x > left && msg->x < left + width && msg->y > top && msg->y < top + height)
        if (msg->lbutton)
            return true;

    return false;
}

// ��������
void DrawChessPieces()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            switch (board[i][j])
            {
            case Player::NONE:
                continue;

            case Player::BLACK_PLAYER:
                setfillcolor(BLACK);
                break;

            case Player::WHITE_PLAYER:
                setfillcolor(WHITE);
                break;
            }

            solidcircle(GRID_LEFT + i * GRID_SIZE, j * GRID_SIZE, 10);
        }
    }
}

// ��������
void DrawChessboard()
{
    setlinecolor(BLACK);
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        line(GRID_LEFT + GRID_SIZE * i, 0, GRID_LEFT + GRID_SIZE * i, SCREEN_HEIGHT);
    }
    for (int i = 0; i <= SCREEN_HEIGHT; i += GRID_SIZE)
    {
        line(GRID_LEFT, i, GRID_LEFT + SCREEN_HEIGHT, i);
    }
}

// ����ʤ���ı�
void DrawSuccessText(Player success)
{
    settextcolor(BLACK);
    switch (success)
    {
    case Player::BLACK_PLAYER:
        outtextxy(10, 50, _T("����ʤ��"));
        break;
    case Player::WHITE_PLAYER:
        outtextxy(10, 50, _T("����ʤ��"));
        break;
    case Player::NONE:
    default:
        break;
    }
}

// �����ֵ�˭���ı�
void DrawPlayText(Player player)
{
    settextcolor(BLACK);
    switch (player)
    {
    case Player::BLACK_PLAYER:
        outtextxy(10, 50, _T("�ֵ�����"));
        break;
    case Player::WHITE_PLAYER:
        outtextxy(10, 50, _T("�ֵ�����"));
        break;
    case Player::NONE:
    default:
        break;
    }
}

// �ж�ʤ��
bool judge(int newX, int newY)
{
    int score, x, y;

    // ͳ�ƺ���
    score = -1;
    y = newY;
    for (x = newX; x >= 0 && board[x--][y] == board[newX][newY]; score++);
    for (x = newX; x < BOARD_SIZE && board[x++][y] == board[newX][newY]; score++);
    if (score >= 5)
        return true;
    // ͳ������
    score = -1;
    x = newX;
    for (y = newY; y >= 0 && board[x][y--] == board[newX][newY]; score++);
    for (y = newY; y < BOARD_SIZE && board[x][y++] == board[newX][newY]; score++);
    if (score >= 5)
        return true;
    // ͳ�����Խ�
    score = -1;
    for (x = newX, y = newY; x >= 0 && y >= 0 && board[x--][y--] == board[newX][newY]; score++);
    for (x = newX, y = newY; x < BOARD_SIZE && y < BOARD_SIZE && board[x++][y++] == board[newX][newY]; score++);
    if (score >= 5)
        return true;
    // ͳ�Ƹ��Խ�
    score = -1;
    for (x = newX, y = newY; x >= 0 && y < BOARD_SIZE && board[x--][y++] == board[newX][newY]; score++);
    for (x = newX, y = newY; x < BOARD_SIZE && y >= 0 && board[x++][y--] == board[newX][newY]; score++);
    if (score >= 5)
        return true;

    return false;
}

// ��ʼ������
void initBoard()
{
    // ��ʼ������
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = Player::NONE;
}

int main()
{
    initBoard();

    HWND hwnd = initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);// ��ʼ��ͼ��ģʽ
    SetWindowTextA(hwnd, "������");

    Player nowPlayer = Player::BLACK_PLAYER;
    Player success = Player::NONE;

    Button* restartButton = new Button(680, 300, 80, 50, _T("���¿�ʼ"));

    while (true)
    {
        BeginBatchDraw();// ��ʼ������ͼ

        setbkcolor(BACKGROUND);// ����ɫ
        cleardevice();// �ñ���ɫ�����Ļ

        DrawChessboard();

        // ��������
        EASYXMSG msg;
        if (peekmessage(&msg, true))
        {
            if (msg.lbutton)
            {
                // ������굥���ĸ���
                int gridX = (msg.x - GRID_LEFT + GRID_SIZE / 2) / GRID_SIZE;
                int gridY = (msg.y + GRID_SIZE / 2) / GRID_SIZE;

                // �ж��Ƿ�Ϊ��Ч����
                if (0 <= gridX && gridX < BOARD_SIZE && 0 <= gridY && gridY < BOARD_SIZE)
                {
                    if (board[gridX][gridY] == Player::NONE && success == Player::NONE)
                    {
                        board[gridX][gridY] = nowPlayer;

                        // �ж�ʤ��
                        if (judge(gridX, gridY))
                            success = nowPlayer;

                        nowPlayer = nowPlayer == Player::BLACK_PLAYER ? Player::WHITE_PLAYER : Player::BLACK_PLAYER;
                    }
                }
            }

            // ������¿�ʼ��ť
            if (restartButton->isClick(&msg))
            {
                initBoard();
                success = Player::NONE;
                nowPlayer = Player::BLACK_PLAYER;
            }
        }

        // ���������ϵ�����
        DrawChessPieces();

        // ����ʤ���ı�
        DrawSuccessText(success);

        // �����ֵ�˭���ı�
        if (success == Player::NONE)
            DrawPlayText(nowPlayer);

        // �������¿�ʼ��ť
        restartButton->Draw();

        FlushBatchDraw();// ˢ�»���
    }

    closegraph();// �ر�ͼ��ģʽ

    return 0;
}
