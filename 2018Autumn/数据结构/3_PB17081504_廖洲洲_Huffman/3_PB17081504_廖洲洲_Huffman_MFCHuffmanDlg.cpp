
// MFCHuffmanDlg.cpp: 实现文件
//


#include "stdafx.h"
#include "MFCHuffman.h"
#include "MFCHuffmanDlg.h"
#include "afxdialogex.h"
#include<stdio.h>
#include<stdlib.h>
#include<process.h>
#include<string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct HTNode {
	unsigned char date; //总共256个字符
	unsigned long weight;
	unsigned short int parent;
	unsigned short int lchild;
	unsigned short int rchild;
}HTNode;

typedef struct htnode {
	unsigned char date; //总共256个字符
//	unsigned short int parent ;
	unsigned short int lchild;
	unsigned short int rchild;
}htnode;

HTNode HT[520], Node;
char **HC;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCHuffmanDlg 对话框



CMFCHuffmanDlg::CMFCHuffmanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCHUFFMAN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCHuffmanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCHuffmanDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDOK, &CMFCHuffmanDlg::OnBnClickedOk)
//	ON_BN_CLICKED(IDCANCEL, &CMFCHuffmanDlg::OnBnClickedCancel)
//	ON_NOTIFY(BCN_DROPDOWN, IDOK, &CMFCHuffmanDlg::OnDropdownIdok)
ON_BN_CLICKED(IDOK, &CMFCHuffmanDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDCANCEL, &CMFCHuffmanDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCHuffmanDlg 消息处理程序

BOOL CMFCHuffmanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCHuffmanDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCHuffmanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCHuffmanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void Select(int n, int *s1, int *s2);
void BubbleSort(HTNode *r, int n);
void HuffmanCoding(char *infile);
void HuffmanDecoding(char *infile);

void CMFCHuffmanDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog OpenFile(TRUE, NULL, NULL, 6UL, _T("All Files(*.*)|*.*"), this);// TODO: 在此添加控件通知处理程序代码
	CString pathname;
	char *p;
	p = new char[200];
	if (OpenFile.DoModal() == IDOK) {
		pathname = OpenFile.GetPathName();
		USES_CONVERSION;
		p = T2A(pathname);
	}
	HuffmanCoding(p);
}




void CMFCHuffmanDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog OpenFile(TRUE, _T("huff"), NULL, 6UL, _T("Huffman Files(*.huff)|*.huff"), this);// TODO: 在此添加控件通知处理程序代码
	CString pathname;
	char *p;
	p = new char[200];
	if (OpenFile.DoModal() == IDOK) {
		pathname = OpenFile.GetPathName();
		USES_CONVERSION;
		p = T2A(pathname);
	}
	HuffmanDecoding(p);
}



void HuffmanCoding(char *infile) {
	char outfile[200]="";   //文件名
	FILE *infp, *outfp;
//	printf("Please input the file name: \n");
	//gets(infile);
	//printf("压缩中！\n");
	if ((infp = fopen(infile, "rb")) == NULL) {
		printf("can't open this file!\n ");
		exit(0);
	}   //打开文件

	strcpy(outfile, infile);
	strcat(outfile, ".huff");
//	char suffix[] = ".huff"; //后缀为.huff
//	strcat(outfile, suffix);
	if ((outfp = fopen(outfile, "wb")) == NULL) {
		printf("can't creat the file!\n ");
		exit(0);
	}    //创建压缩文件

	//开始第一遍扫描文件，创建Huffman树
	for (int i = 0; i < 520; i++) {
		HT[i].weight = 0;
		HT[i].lchild = 0;
		HT[i].parent = 0;
		HT[i].rchild = 0;
	}
	unsigned long length = 0;
	unsigned char c;
	while (fread(&c, 1, 1, infp)) {
		HT[c + 1].weight++;
		length++;
		//		printf("1");
		//		if (c == 0)
		//			printf("文件中有NULL！\n");
	}  //统计完成
//	printf("%d\n",length);
	short int charcount = 0;  //统计字符种类数
	for (int i = 1; i <= 256; i++) {
		if (HT[i].weight) {
			HT[i].date = (unsigned char)(i - 1);
			charcount++;
		}

		//	else
		//			HT[i].date = NULL;
	}//给统计到的字符存储在节点中
	//下面将在文件中存在的字符节点放在数组前面，便于建树
	//采用冒泡排序
	printf("#####charcount=%d\n", charcount);
	BubbleSort(HT + 1, 257); //冒泡排序完成
	int m = 2 * charcount - 1;
	int s1 = 1, s2 = 1;
	for (int i = charcount + 1; i <= m; ++i) {
		//在HT【1...i】选择perent为0且weight最小的两个结点，取序号分别为s1和s2
		Select(i - 1, &s1, &s2);
		HT[s1].parent = i; HT[s2].parent = i;
		HT[i].lchild = s1; HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	//从叶子到根逆向求每个字符的Huffman编码
	char *cd;
	int start;
	int j, f;
	HC = (char * *)malloc((charcount + 1) * sizeof(char*));
	cd = (char *)malloc(charcount * sizeof(char)); //存储每次读出的编码
	cd[charcount - 1] = '\0';

	for (int i = 1; i <= charcount; i++) {
		start = charcount - 1;//编码结束符位置
		for (j = i, f = HT[i].parent; f != 0; j = f, f = HT[f].parent) {//从叶子到根逆向求编码
			if (HT[f].lchild == j)
				cd[--start] = '0';
			else
				cd[--start] = '1';
		}
		HC[i] = (char*)malloc((charcount - start) * sizeof(char));
		strcpy(HC[i], cd + start);
		printf("%s\n", HC[i]);
	}
	free(cd);
	//	int k=strlen(HC[1]);
	for (int i = 1; i <= charcount; i++) {
		printf("%c...", HT[i].date);
		printf("%s\n", HC[i]);
	}
	htnode *ht;
	ht = (htnode *)malloc(2 * charcount * sizeof(htnode));
	for (int i = 0; i < 2 * charcount; i++) {
		ht[i].date = HT[i].date;
		ht[i].lchild = HT[i].lchild;
		//	ht[i].parent=HT[i].parent;
		ht[i].rchild = HT[i].rchild;
	}
	//	printf("ok!\n");
		//编码完成，下面扫描文件第二遍，将Huffman编码写入
	fwrite(&charcount, sizeof(short int), 1, outfp);
	fwrite(ht, sizeof(htnode), 2 * charcount, outfp);//将Huffman树写入压缩文件头
	short int num0 = 0;
	//	fseek(outfp,sizeof(int),SEEK_CUR);//将读写指针向尾移动存储int的空间，用来存储加入的0个数
	fwrite(&num0, sizeof(short int), 1, outfp);
	fseek(infp, 0, SEEK_SET);//将读写指针移到文件开头
	char code[300];//记录每次翻译的编码，最长的编码不会超过256位
	code[0] = '\0';
	//	memset(code,'\0',300*sizeof(char));
	int i;
	int codelength;

	while (fread(&c, 1, 1, infp)) {
		for (i = 1; i <= charcount; i++) {
			if (c == HT[i].date)
				break;
		}
		//		int len=strlen(HC[i]);
		//		printf("%d\n",len);
		strcat(code, HC[i]);
		codelength = strlen(code);
		c = 0;
		while (codelength >= 8) {
			for (int i = 0; i < 8; i++) {
				if (code[i] == '1')    //c用来暂时保存新的编码，经过8位移位，其二进制码变为新编码
					c = (c << 1) | 1;     //向左移一位，最后一位通过或加1
				else
					c = c << 1;       //直接左移，自动补0
			}
			fwrite(&c, sizeof(char), 1, outfp); //写入压缩文件
			strcpy(code, code + 8);
			codelength = strlen(code);
		}
	}
	if (codelength > 0) { //若最后未满8位，补零
		num0 = 8 - codelength;
		strcat(code, "00000000");//补0
		c = 0;
		for (i = 0; i < 8; i++) {
			if (code[i] == '1')
				c = (c << 1) | 1;
			else
				c = c << 1;
		}
		fwrite(&c, sizeof(char), 1, outfp);
	}  //写入完成

	fseek(outfp, 2 * charcount * sizeof(htnode) + sizeof(short int), SEEK_SET);
	fwrite(&num0, sizeof(short int), 1, outfp);
	printf("压缩完成！\n");
	printf("%d@@@@%d@@@@\n", charcount, num0);
	fclose(infp);
	fclose(outfp);
	if ((outfp = fopen(outfile, "rb")) == NULL) {
		printf("can't creat the file!\n ");
		exit(0);
	}    //创建压缩文件
	short int l;
	fread(&l, sizeof(short int), 1, outfp);
	printf("%d\n", l);
	fseek(outfp, 2 * charcount * sizeof(htnode) + sizeof(short int), SEEK_SET);
	fread(&l, sizeof(short int), 1, outfp);
	printf("%d\n", l);
	fread(&l, sizeof(short int), 1, outfp);
	printf("%d\n", l);
	fclose(outfp);
	getchar();
}
void BubbleSort(HTNode *r, int n) {
	int i, j, lastExchange;
	HTNode temp;
	i = n;
	while (i > 0) {
		lastExchange = 0;
		for (j = 1; j < i; j++) {
			if (r[j - 1].weight < r[j].weight) {
				temp = r[j];
				r[j] = r[j - 1];
				r[j - 1] = temp;
				lastExchange = j;
			}
		}
		i = lastExchange;
	}
}
void Select(int n, int *s1, int *s2) {
	unsigned long min = HT[n].weight;
	int i;
	*s1 = n;
	for (i = 1; i <= n; i++) {
		if (HT[i].parent)
			;
		else if (HT[i].weight < min) {
			min = HT[i].weight;
			*s1 = i;
		}
	}
	for (i = 1; i <= n && HT[i].parent != 0; i++);
	if (i == *s1)
		for (i = *s1 + 1; i <= n && HT[i].parent != 0; i++);
	unsigned long min2 = HT[i].weight;
	*s2 = i;
	for (i = 1; i <= n; i++) {

		if (i == *s1 || HT[i].parent)
			;
		else if (HT[i].weight < min2) {
			min2 = HT[i].weight;
			*s2 = i;
		}
	}


}

void HuffmanDecoding(char *infile) {
	char  outfile[200]="";   //文件名
	FILE *infp, *outfp;
//	printf("Decompression---Please input the file name: \n");
//	gets(infile);
//	printf("%s", infile);
	if ((infp = fopen(infile, "rb")) == NULL) {
		printf("can't open this file!\n ");
		exit(0);
	}   //打开文件
//	printf("解压中！\n");

	if (strstr(infile, ".huff") == NULL) {
		printf("文件格式错误！\n");
		exit(0);
	}
//	int infilenamelen;
//	infilenamelen = strlen(infile);
//	strncpy(outfile, infile, infilenamelen - 5);
	strncpy(outfile, infile, strlen(infile) - 5);
	outfile[strlen(infile) - 5] = '\0';

	if ((outfp = fopen(outfile, "wb")) == NULL) {
		printf("can't creat the file!\n ");
		exit(0);
	}    //创建解压文件
	short	int charcount;
	short	int num0;
	int i;
	fread(&charcount, sizeof(short int), 1, infp);
	htnode *ht;
	ht = (htnode *)malloc(2 * charcount * sizeof(htnode));
	HTNode *HT;
	HT = (HTNode *)malloc(2 * charcount * sizeof(HTNode));
	//	int x=sizeof(char);
	//	int y=2*charcount*sizeof(htnode);
	//	printf("%d ########  %d",x,y);
	for (i = 0; i < 2 * charcount; i++)
		fread(&ht[i], sizeof(htnode), 1, infp);
	for (i = 0; i < 2 * charcount; i++) {
		HT[i].date = ht[i].date;
		HT[i].lchild = ht[i].lchild;
		HT[i].rchild = ht[i].rchild;

	}
	for (i = 1; i < 2 * charcount; i++) {
		HT[HT[i].lchild].parent = i;
		HT[HT[i].rchild].parent = i;
	}
	fread(&num0, sizeof(short int), 1, infp);
	for (i = 1; i <= charcount; i++) {
		printf("%c...", HT[i].date);
	}
	printf("%d@@@@%d@@@@@\n", charcount, num0);

	//	char code[300];
	char buffer[9];
	char code[9];
	buffer[0] = '\0';
	code[0] = '\0';
	unsigned char c;
	unsigned char date;
	long j;
	int codelength = 0;
	int flag = 1;  //如果flag=1，说明从根开始
	int len0;
	HTNode node;
	HTNode rootnode = HT[2 * charcount - 1];
	while (fread(&c, sizeof(char), 1, infp)) {
		j = (long)c;
		itoa(j, buffer, 2);		   //将c转换位二进制,由于转换后可能后面不是8位，需要在前面补零
		codelength = strlen(buffer);
		len0 = 8 - codelength;
		for (int i = 0; i < len0; i++)
			strcat(code, "0");
		strcat(code, buffer);


		if (feof(infp)) {
			for (int i = 0; i < 8 - num0; i++) {
				if (code[i] == '1') {
					if (flag == 1) {
						flag = 0;
						node = HT[rootnode.rchild];
					}
					else
						node = HT[node.rchild];
				}
				else {
					if (flag == 1) {
						flag = 0;
						node = HT[rootnode.lchild];
					}
					else
						node = HT[node.lchild];
				}
				if (!node.lchild && !node.rchild) {//读到数据点
					date = node.date;
					fwrite(&date, sizeof(char), 1, outfp);
					flag = 1;
				}
			}

		}




		else {

			//		strcat(code,buffer);
			//		codelength=strlen(code);
			for (int i = 0; i < 8; i++) {
				if (code[i] == '1') {
					if (flag == 1) {
						flag = 0;
						node = HT[rootnode.rchild];
					}
					else
						node = HT[node.rchild];
				}
				else {
					if (flag == 1) {
						flag = 0;
						node = HT[rootnode.lchild];
					}
					else
						node = HT[node.lchild];
				}
				if (!node.lchild && !node.rchild) {//读到数据点
					date = node.date;
					fwrite(&date, sizeof(char), 1, outfp);
					flag = 1;
				}
			}
		}
		code[0] = '\0';
		buffer[0] = '\0';
	}


	fclose(infp);
	fclose(outfp);
	getchar();
}


//void CMFCHuffmanDlg::OnDropdownIdok(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMBCDROPDOWN pDropDown = reinterpret_cast<LPNMBCDROPDOWN>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}




