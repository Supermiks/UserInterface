#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

IplImage* image = 0;
IplImage* src = 0;

class Button
{
private:
	int x; int y;
	string buttonSrc;
public:
	Button()
		: x(0), y(0), buttonSrc("On_Button.png") { }

	Button(const int _x, const int _y, string _buttonSrc)
		: x(_x), y(_y), buttonSrc(_buttonSrc) { }

	Mat imgButton = imread(buttonSrc);
	Rect rectButton = Rect(x, y, imgButton.cols, imgButton.rows);
};

class CheckBox
{
private:
	int x; int y;
	string checkBoxOnSrc; string checkBoxOffSrc;
public:
	CheckBox()
		:x(0), y(0), checkBoxOnSrc("CheckBoxON.png"), checkBoxOffSrc("CheckBoxOFF.png") { }

	CheckBox(const int _x, const int _y, string _checkBoxOnSrc, string _checkBoxOffSrc)
		:x(_x), y(_y), checkBoxOnSrc(_checkBoxOnSrc), checkBoxOffSrc(_checkBoxOffSrc) { }

	bool checkBoxFlag = true;

	Mat imgCheckBox = imread(checkBoxOnSrc);
	Rect rectCheckBox = Rect(x, y, imgCheckBox.cols, imgCheckBox.rows);
};

class Mouse
{
public:
	void myMouseCallback(int event, int x, int y, int flags);
};

Mouse* g_ptr;
void onMouse(int event, int x, int y, int flags, void* param)
{
	g_ptr->myMouseCallback(event, x, y, flags);
}

class UserInterface
{
private:
	
public:
	Mouse mouseCallBack;
	Mat canvas;

	Button *buttons; CheckBox *checkboxes;
	const int BUTTONS_COUNT; const int CHECKBOXES_COUNT;

	const string BUTTON_SRC;
	const string CHECKBOX_ON_SRC; const string CHECKBOX_OFF_SRC;

	UserInterface(Button _buttons[], CheckBox _checkboxes[], int buttonsCount, int checkBoxesCount,
		string buttonSrc, string checkBoxOnSrc, string checkBoxOffSrc)
		:BUTTONS_COUNT(buttonsCount), CHECKBOXES_COUNT(checkBoxesCount),
		BUTTON_SRC(buttonSrc), CHECKBOX_ON_SRC(checkBoxOnSrc), CHECKBOX_OFF_SRC(checkBoxOffSrc)
	{
		buttons = new Button[BUTTONS_COUNT];
		checkboxes = new CheckBox[CHECKBOXES_COUNT];

		for (int i = 0; i < BUTTONS_COUNT; i++)
		{
			buttons[i] = _buttons[i];
		}
		for (int i = 0; i < CHECKBOXES_COUNT; i++)
		{
			checkboxes[i] = _checkboxes[i];
		}

		g_ptr = &mouseCallBack;

		canvas = Mat(300, 300, CV_8UC3, Scalar(0, 0, 0));
		for (int n = 0; n < BUTTONS_COUNT; n++)
		{
			buttons[n].imgButton.copyTo(canvas(buttons[n].rectButton));
		}

		for (int n = 0; n < CHECKBOXES_COUNT; n++)
		{
			checkboxes[n].imgCheckBox.copyTo(canvas(checkboxes[n].rectCheckBox));
		}
	}
};

UserInterface *userInterfaceInstance;
UserInterface usInter = *userInterfaceInstance;

// обработчик событий от мышки
void Mouse::myMouseCallback(int event, int x, int y, int flags)
{
	switch (event) {
	case CV_EVENT_MOUSEMOVE:
		break;

	case CV_EVENT_LBUTTONDOWN:
		if (event == EVENT_LBUTTONDOWN)
		{
			for (int n = 0; n < usInter.BUTTONS_COUNT; n++)
			{
				if (usInter.buttons[n].rectButton.contains(Point(x, y)))
				{
					cout << "Button " << n + 1 << " clicked!" << endl;
				}
			}

			for (int n = 0; n < usInter.CHECKBOXES_COUNT; n++)
			{
				if (usInter.checkboxes[n].rectCheckBox.contains(Point(x, y)))
				{
					cout << "CheckBox " << n + 1 << " clicked!" << endl;

					if (usInter.checkboxes[n].checkBoxFlag)
					{
						usInter.checkboxes[n].checkBoxFlag = false;
						usInter.checkboxes[n].imgCheckBox = imread(usInter.CHECKBOX_OFF_SRC);
						usInter.checkboxes[n].imgCheckBox.copyTo(usInter.canvas(usInter.checkboxes[n].rectCheckBox));
					}
					else
					{
						usInter.checkboxes[n].checkBoxFlag = true;
						usInter.checkboxes[n].imgCheckBox = imread(usInter.CHECKBOX_ON_SRC);
						usInter.checkboxes[n].imgCheckBox.copyTo(usInter.canvas(usInter.checkboxes[n].rectCheckBox));
					}
				}
			}

			printf("%d x %d\n", x, y);
			break;
		}

	case CV_EVENT_LBUTTONUP:
		break;
	}
}

int main(int argc, char* argv[])
{
	const int BUTTONS_COUNT = 2;
	const int CHECKBOXES_COUNT = 2;

	const string BUTTON_SRC = "On_Button.png";
	const string CHECKBOX_ON_SRC = "CheckBoxON.png"; const string CHECKBOX_OFF_SRC = "CheckBoxOFF.png";

	Button buttons[BUTTONS_COUNT]{ Button(0,0,BUTTON_SRC), Button(100,0,BUTTON_SRC) };
	CheckBox checkboxes[CHECKBOXES_COUNT]{ CheckBox(0,100,CHECKBOX_ON_SRC, CHECKBOX_OFF_SRC), CheckBox(150,100,CHECKBOX_ON_SRC, CHECKBOX_OFF_SRC) };

	userInterfaceInstance = new UserInterface(buttons, checkboxes, BUTTONS_COUNT, CHECKBOXES_COUNT, BUTTON_SRC, CHECKBOX_ON_SRC, CHECKBOX_OFF_SRC);

	// окно для отображения картинки
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);

	// вешаем обработчик мышки
	cvSetMouseCallback("original", onMouse, 0);

	while (1) {
		// показываем картинку
		imshow("original", usInter.canvas);

		char c = cvWaitKey(33);
		if (c == 27) { // если нажата ESC - выходим
			break;
		}
	}

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&src);
	// удаляем окно
	cvDestroyWindow("original");
	return 0;
}