
// DrawDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "Draw.h"
#include "DrawDlg.h"
#include "afxdialogex.h"
#include"functionsForAprocsimation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// диалоговое окно CDrawDlg
CDrawDlg::CDrawDlg(CWnd* pParent /*=NULL*/): CDialogEx(IDD_DRAW_DIALOG, pParent)

	, s_edit(0)
	, last_m(0)
	, x_buff(nullptr)
	, y_buff(nullptr)
	, first_initialization(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _m);
	DDX_Control(pDX, IDC_EDIT2, _n);
	DDX_Control(pDX, IDC_EDIT3, _S);
	DDX_Text(pDX, IDC_EDIT3, s_edit);
}

BEGIN_MESSAGE_MAP(CDrawDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDrawDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_PICTURE, &CDrawDlg::OnStnClickedPicture)
END_MESSAGE_MAP()


// обработчики сообщений CDrawDlg

BOOL CDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, TRUE);		// Мелкий значок

	drv.Create(GetDlgItem(IDC_PICTURE)->GetSafeHwnd());
	_S.SetReadOnly(TRUE);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CDrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDrawDlg::OnBnClickedOk()
{
	//Блок основных вычислений////////////////////////////////////////////////////////////////////////
	int m(0), n(0);
	m = GetDlgItemInt(IDC_EDIT1);
	n = GetDlgItemInt(IDC_EDIT2);
	srand(time(NULL));
	double a(-1), b(1), z(0);
	int c = 0;
	double* x = new double[m + 1];
	double* y = new double[m + 1];
	double* zar = new double[m + 1];
	double* car = new double[m + 1];


	if (m == last_m)
	{
		for (int i = 0; i <= m; i++)
		{
			x[i] = x_buff[i];
			y[i] = y_buff[i];
		}
	}
	else
	{
		if (first_initialization == true)
		{
			x_buff = new double[m + 1];
			y_buff = new double[m + 1];
			first_initialization = false;
		}
		
		for (int i = 0; i <= m; i++)
		{
			z = (double)rand() / RAND_MAX;
			//z_buff[i] = z;
			c = 1 + rand() % 5;
			//c_buff[i] = c;
			x[i] = a + i * (b - a) / m;
			x_buff[i] = x[i];
			if (z < 0.5)							// таблично задаем функцию
			{ 
				y[i] = f(x[i]) * (1 - z / c);
				y_buff[i] = y[i];
			}
			else 
			{ 
				y[i] = f(x[i]) * (1 + z / c);
				y_buff[i] = y[i];
			}
		}
	}
	last_m = m;
	double *C = new double[n + 1];
	double *D = new double[2 * n + 1];

	//-------------------------------------------------------------------------------------------	
	for (int k = 0; k <= 2 * n; k++)		//Заполнение массива D
	{
		D[k] = 0;
		for (int i = 0; i <= m; i++)
		{
			D[k] += pow(x[i], k);
		}
	}
//-------------------------------------------------------------------------------------------
	double **A = new double *[n + 1];			// Заполнение массива А
	for (int i = 0; i <= n; i++)
	{
		A[i] = new double[n + 1];
	}
	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			A[i][j] = D[i + j];
		}
	}
	//-------------------------------------------------------------------------------------------
	double* B = new double[n + 1];				//Заполнение массива В
	for (int k = 0; k <= n; k++)
	{
		B[k] = 0;
		for (int i = 0; i <= m; i++)
		{
			B[k] += y[i] * pow(x[i], k);
		}
	}
	//-------------------------------------------------------------------------------------------
	double **A1 = new double* [n + 1];			// Заполнение буферного массива А
	for (int i = 0; i <= n; i++)
	{
		A1[i] = new double[n + 1];
	}
	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			A1[i][j] = A[i][j];
		}
	}
	//-------------------------------------------------------------------------------------------
	double *B1 = new double[n + 1];				// Заполнение буферного массива В
	for (int k = 0; k <= n; k++)
	{
		B1[k] = B[k];
	}
	Gauss(A, C, B, n, m);
	//-------------------------------------------------------------------------------------------
	double delta = 0;
	for (int i = 0; i <= n; i++)				// Невязки
	{
		delta = 0;
		for (int j = 0; j <= n; j++)
		{
			delta += A1[i][j] * C[j];
		}
		delta -= B1[i];
	}
	double S = S_Calc(n, m, x, y, C);
	UpdateData(TRUE);
	s_edit = S;
	UpdateData(FALSE);

	//Блок заполнения и передачи векторов в функцию для рисования ////////////////////////////////////////////////////////////////////////
	vector<double> f_x;
	vector<double> f_y;
	vector<double> F_X;
	vector<double> F_Y;
	vector<double> Ellipse_x;
	vector<double> Ellipse_y;
	double buffer_Y(0);
	for (double i = -2; i < 100; i += 0.04)
	{
		f_y.push_back(f(i));
		f_x.push_back(i);
		buffer_Y = C[0];
		for (int j = 1; j <= n; j++)
		{
			buffer_Y += C[j] * pow(i, j);
		}
		F_X.push_back(i);
		F_Y.push_back(buffer_Y);
	}
	for (int i = 0; i <= m; i++)
	{
		Ellipse_x.push_back(x[i]);
		Ellipse_y.push_back(y[i]);
	}
	drv.Draw(f_y, F_Y, Ellipse_y,f_x, F_X, Ellipse_x);

	//Запись в файл////////////////////////////////////////////////////////////////////////
	ofstream fileObj; //Объект для записи результата в файл
	fileObj.open("notebook.txt", std::ios_base::app); //Связываем объект с файлом 
	fileObj.precision(9);
	fileObj << "S = " << S << endl << "n = " << n << endl << "m = " << m << endl;
	for (int i = 0; i <= n; i++)
	{
		fileObj << "C" << i << " = " << C[i] << endl;
	}

	fileObj << endl << endl;

	fileObj << "Невязки: ";
	for (int i = 0; i <= n; i++)				// Невязки
	{
		delta = 0;
		for (int j = 0; j <= n; j++)
		{
			delta += A1[i][j] * C[j];
		}
		delta -= B1[i];
		fileObj << "delta(" << i << ") = "  << delta << "; ";
	}

	fileObj << endl << endl;

	fileObj << "Y(x) = ";
	for (int i = n; i >= 0; --i)
	{
		fileObj << C[i] << "x^" << i;
		if (i != 0) fileObj << "+";
	}
	fileObj << endl << endl;
	for (int i = 0; i <= m; i++)
	{
		fileObj << "z" << i << " = " << zar[i] << "    c" << i << " = " << car[i] << endl;
	}
	for (int i = 0; i <= m; i++)
	{
		fileObj << "{" << x[i] << ',' << y[i] << "}";
		if (i != m) fileObj << ',';
	}
	fileObj << endl << "========================================================================================================" << endl;

	//Oчистка памяти////////////////////////////////////////////////////////////////////////
	Ellipse_x.clear();
	Ellipse_y.clear();
	F_X.clear();
	F_Y.clear();
	f_x.clear();
	f_y.clear();
	for (int i = 0; i <= n; i++)
	{
		delete[] A[i];
		delete[] A1[i];
		A1[i] = nullptr;
		A[i] = nullptr;
	}
	delete[] A;
	delete[] A1;
	delete[] x;
	delete[] y;
	delete[] D;
	delete[] C;
	delete[] B;
	delete[]B1;
	x = nullptr;
	y = nullptr;
	D = nullptr;
	C = nullptr;
	B = nullptr;
	A = nullptr;
	A1 = nullptr;
	B1 = nullptr;
}

void CDrawDlg::OnStnClickedPicture()
{
	// TODO: добавьте свой код обработчика уведомлений
}
