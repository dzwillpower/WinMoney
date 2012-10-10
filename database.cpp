/*
���Ӽ������1.0�������а棩
���ݿ⴦��ģ���ʵ��
����:zwdnet@163.com
����:2011-10-16
���Э�飺GPL3
�����޸ģ�2012-10-05��ֲ��windows��
*/
#include "database.h"

DataBase::DataBase()
{
	databaseName = "money.db"; //ָ�����ݿ��ļ���
	db = NULL;
	errMsg = "";
	bOpen = false; //��ʼʱ���ݿ�ر�
}

DataBase::~DataBase()
{
	//����������ݿ⣬�ر�
	if (db != NULL)
	{
		closeDataBase();
	}
	//��������˽���ռ䣬�ջ�
	if (result.result != NULL)
	{
		sqlite3_free_table(result.result);
		result.row = 0;
		result.col = 0;
	}
}

//ɾ�����ݿ��ļ�������ɾ���������ݣ����³�ʼ��
void DataBase::rmDataBase(void)
{
	//������ļ����ڣ�ɾ��֮��linux����Ч������ϵͳӦ��Ҫ�޸�
	if (access(databaseName, F_OK) != -1)
	{
		system("del money.db");
	}
}

//�����ݿ�
bool DataBase::openDataBase(void)
{
	int rc = sqlite3_open(databaseName, &db);
	if (rc)
	{
		errMsg = const_cast<char*>(sqlite3_errmsg(db));
		return false;
	}
	bOpen = true;
	return true;
}

//�ر����ݿ�
bool DataBase::closeDataBase(void)
{
	if (bOpen)
	{
		sqlite3_close(db);
		bOpen = false;
		return true;
	}
	//�����ݿ��Ǵ򿪵ĵ������ǵ����˵���ر�����
	if (bOpen)
	{
		return false;
	}
	//����˵�����ݿⱾ������û�򿪵ġ�
	return true;
}

//�������ݿ⼰��ر����ݿ⽨ģ���ṹ�뿴�ĵ�
bool DataBase::createDataBase(void)
{
	//������ݿ�δ�򿪣��ȴ����ݿ⣨���û�и����ݿ����Զ�������
	if (!bOpen)
	{
		if (!openDataBase())
		{
			return false;
		}
	}

	//��������֧����
	string sql = "CREATE TABLE Income( \
		      ID INTEGER PRIMARY KEY, \
		      Time DATE, \
		      Name NVARCHAR, \
		      Amount DECIMAL(7,2), \
		      TypeID INTEGER,  \
		      Remark NVARCHAR);";
	sqlite3_stmt * statement;
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "��������֧�������!";
		return false;
	}
	int success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "��������֧�������!";
		return false;
	}

	//��������֧�������
	sql = "CREATE TABLE IncomeType( \
	       TypeID INTEGER PRIMARY KEY, \
	       TypeName NVARCHAR);";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "��������֧����������!";
		return false;
	}
	success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "��������֧����������!";
		return false;
	}

	//�����ʲ���ծ��
	sql = "CREATE TABLE Investment( \
		      ID INTEGER PRIMARY KEY, \
		      Time DATE, \
		      Name NVARCHAR, \
		      Amount DECIMAL(7,2), \
		      TypeID INTEGER,  \
		      Remark NVARCHAR);";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "�����ʲ���ծ�����!";
		return false;
	}
	success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "�����ʲ���ծ�����!";
		return false;
	}

	//�����ʲ���ծ�����
	sql = "CREATE TABLE InvestmentType( \
	       TypeID INTEGER PRIMARY KEY, \
	       TypeName NVARCHAR);";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "�����ʲ���ծ��������!";
		return false;
	}
	success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "�����ʲ���ծ��������!";
		return false;
	}

	//�����籣��ֻ���浽������ڵĸ��˿۳�ֵ�ܶ�
	sql = "CREATE TABLE SocietyInsurance( \
	       Time DATE, \
	       Old DECIMAL(7,2),\
	       Med DECIMAL(7,2),\
	       Job DECIMAL(7,2),\
	       House DECIMAL(7,2));";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "�����籣���˽ɷѱ����!";
		return false;
	}
	success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "�����籣���˽ɷѱ����!";
		return false;
	}
	//��ʼ����ʱ���ȴ���һ�������ݣ�����������
	sql = "insert into SocietyInsurance values(19800101, 0.0, 0.0, 0.0, 0.0);";
	if (!modifyDataBase(sql))
	{
		errMsg = "�����籣���˽ɷѱ����!";
		return false;
	}

	closeDataBase();
	return true;
}

//��ȡ������Ϣ
char * DataBase::getErrorMessage(void)
{
	return errMsg;
}

//��ʼ�����ݿ�
bool DataBase::initDataBase(void)
{
	rmDataBase();
	createDataBase();
}

//�޸����ݿ⣨���롢ɾ����
bool DataBase::modifyDataBase(const string & querySQL)
{
	if (!bOpen)
	{
		openDataBase();
	}
	sqlite3_stmt * statement;
	if (sqlite3_prepare_v2(db, querySQL.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "�޸����ݿ�ʧ��!";
		return false;
	}
	int success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "�޸����ݿ�ʧ��!";
		return false;
	}
	closeDataBase();
	return true;
}

//ִ��SQL��ѯ���
bool DataBase::runSQL(const string & querySQL,
	    QueryResult & res)
{
	//�����û�򿪾Ͳ�ѯ���ᱨ��
	if (!bOpen)
	{
		if (!openDataBase())
		{
			return false;
		}
	}
	int rc = sqlite3_get_table(db, querySQL.c_str(), &res.result, &res.row,
			 &res.col, &errMsg);
	if (rc != 0)
	{
		return false;
	}

	if (!closeDataBase())
	{
		return false;
	}
	return true;
}

/*�����ѯ����ṹ���ͷ��ڴ棬���µĽ�����롣��Ϊʹ��
��Ĺ����л������в�ѯ����λ�ý�������ܵȳ���
�������Զ����ýṹ�������������ͷ��ڴ档
*/
void DataBase::clearResult(QueryResult & res)
{
	if (res.row == 0 || res.col == 0 || res.result != NULL)
	{
		sqlite3_free_table(res.result);
		res.row = 0;
		res.col = 0;
	}
}
