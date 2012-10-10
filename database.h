#ifndef DATABASE_H
#define DATABASE_H
/*
���Ӽ������1.0�������а棩
���ݿ⴦��ģ�飬���ڷ�װ��sqlite3���е����ݿ���ɾ��ļ�����ָ���Ĳ�ѯ���Ĳ���
����:zwdnet@163.com
����:2011-10-16
���Э�飺GPL3
�����޸ģ�2012-10-05��ֲ��windows��
*/
#include <sqlite3.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;
//�������ݿ��ѯ����Ľṹ
typedef struct queryresult
{
	int row;        //��ѯ�������
	int col;        //��ѯ�������
	char ** result; //��ѯ����Ķ�ά����
	void clear()
	{
		if (result != NULL)
		{
			sqlite3_free_table(result);
			row = 0;
			col = 0;
		}
	}
	queryresult()   //�ṹ�Ĺ��캯��
	{
		row = 0;
		col = 0;
		result = NULL;
	}
	~queryresult() //������������������˽�����ͷ��ڴ�
	{
		clear();
	}
} QueryResult;

//��װ���ݿ��������
class DataBase
{
public:
	DataBase();
	~DataBase();
	bool initDataBase();          //��ʼ�����ݿ�
	bool openDataBase();          //�����ݿ�
	bool closeDataBase();         //�ر����ݿ�
	bool modifyDataBase(const string & querySQL);//�޸����ݿ⣬����ɾ��
	bool runSQL(const string & querySQL,
		    QueryResult & res); //���в�ѯ���querySQL�������ŵ�res��
	char * getErrorMessage(void); //��ô�����Ϣ
	void clearResult(QueryResult & res); //�����ѯ����ṹ���Թ��µĲ�ѯ�������

private:
	void rmDataBase();     //�ڲ������������ڳ�ʼ����ʱ��ɾ��sqlite3���ݿ��ļ�
	bool createDataBase(); //�������ݿ⼰��صı�

private:
	sqlite3 * db;  //���ݿ�ָ��
	char * errMsg; //���ݿ�����Ĵ�����Ϣ
	bool bOpen;    //��־���ݿ��Ƿ�򿪵ı���
	QueryResult result; //��ѯ���ݿ�Ľ��
	char * databaseName; //���ݿ�����
};
#endif
