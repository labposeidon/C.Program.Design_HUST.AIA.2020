#include "common.h"
#include "lgstate.h"
/*	FileName: report.c
	Author:   ���Ӻ� 
	Date��    2020/9/7
	function��char judge_password(char mima[]) ;�����ʽ
			char judge_IDnumber(char id[]); ����֤�Ÿ�ʽ
			char compare_password(char key1[],char key2[]); ȷ�������Ƿ���ͬ
			char right_id(char account[],char id[]);ȷ������֤�����Ƿ���ͬ
			char judge_same_name(char account[]); �Ƿ�ע��
			char right_password(char account[],char key[]); ������ȷ
			char judge_userfrozen(char account[]); �ж��û��Ƿ񱻶��� 
			char user_freeze(char account[]) ; �����˻� 
			char user_unfreeze(char account[]);�������ܣ����ʻ��ⶳ
			char complete_register(char account,char key1,char key2,char id); ���ע������
			char complete_login(char account,char key); ��ɵ�¼����
			char complete_admin(char key);��ɹ���Ա��¼����
			void wr_user_inf(char account[],char key[],char id[]);��ע����Ϣд���ļ�
			void wr_user(char account[]);�����ε�¼���˺�д���ļ�
			char admin_password(char key[]);�жϹ���Ա��¼ʱ�����Ƿ����*/

/*�������ܣ��ж������Ƿ�Ϻ�Ҫ��
  ����ֵ��0��ʾ���Ȳ��Ϻ�Ҫ��1��ʾ�Ϻ�Ҫ��*/
char judge_password(char mima[])
{
    int i;
    int flag = 1;
    for (i = 0; i < 6; i++)
    {
        if (mima[i] == '\0') //�����볤��С��6���򳤶ȴ���
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

/*�������ܣ��ж�����֤���Ƿ�Ϻ�Ҫ��
  ����ֵ��0��ʾ���Ȳ��Ϻ�Ҫ��1��ʾ���ݲ��Ϻ�Ҫ��2��ʾ�Ϻ�Ҫ��*/
char judge_IDnumber(char id[])
{
    int i;
    int flag = 2;
    for (i = 0; i < 19; i++)
    {
        if (id[i] == '\0') //����ǰ�������򳤶ȴ���
        {
            break;
        }
        if ((i < 17) && (id[i] < '0' || id[i] > '9')) //���ڳ����һλ֮����ַ������ַ��������ݴ���
        {
            flag = 1;
            break;
        }
        if ((i == 17) && (id[i] != 'x' && id[i] != 'X') && (id[i] < '0' || id[i] > '9')) //�������һλ���ֲ���x��X�ķ������ַ��������ݴ���
        {
            flag = 1;
            break;
        }
    }
    if (i < 18 && flag != 1)
    {
        flag = 0;
    }
    return flag;
}

/*�������ܣ��ж���������������Ƿ���ͬ
   ����ֵ��0��ʾ����ͬ��1��ʾ��ͬ*/
char compare_password(char key1[], char key2[])
{
    int i;
    int flag = 0;
    for (i = 0; i < 13; i++)
    {
        if (key1[i] != key2[i])
        {
            flag = 0;
            break;
        }
        if (key1[i] == '\0')
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

/*�������ܣ��ж�ע����˺��Ƿ��������˺�����
  ����ֵ��0��ʾ������1��ʾ������
  �������ܣ��жϵ�¼�˺��Ƿ��Ѿ�ע��
  ����ֵ��0��ʾ��ע�ᣬ1��ʾ��δע��*/
char judge_same_name(char account[])
{
    FILE *fp;
    USER *u = NULL;
    int i, j;
    int flag = 1;
    int length;
    if ((fp = fopen("UserInfo.dat", "rb+")) == NULL)
    {
        printf("\nUserInfo.dat error file");
        delay(3000);
        exit(1); //��ʧ�ܣ��˳�����
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) / sizeof(USER);
    for (i = 0; i < length; i++)
    {
        if ((u = (USER *)malloc(sizeof(USER))) == NULL)
        {
            printf("memory error JudgeSameName");
            delay(3000);
            exit(1); //����ռ䲻�㣬�˳�����
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fread(u, sizeof(USER), 1, fp);
        for (j = 0; j < 13; j++)
        {
            if (u->UserName[j] != account[j])
            {
                break;
            }
            if (account[j] == '\0')
            {
                j = 13;
                break;
            }
        }
        if (j == 13)
        {
            flag = 0;
            break;
        }
        free(u);
        u = NULL;
    }
    if (u != NULL)
    {
        free(u);
        u = NULL;
    }
    //setbkcolor(RED);
    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file UserInfo.");
        delay(3000);
        exit(1);
    }
    return flag;
}

/*�������ܣ��ж��˺������Ƿ�ƥ��
  ����ֵ��0��ʾ����ȷ��1��ʾ��ȷ*/
char right_password(char account[], char key[])
{
    FILE *fp;
    USER *u = NULL;
    int i, j, k = 0;
    int flag = 0;
    int length;
    if ((fp = fopen("UserInfo.dat", "rb+")) == NULL)
    {
        printf("\nUserInfo.dat error file");
        delay(3000);
        exit(1); //��ʧ�ܣ��˳�����
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) / sizeof(USER);
    for (i = 0; i < length; i++)
    {
        if ((u = (USER *)malloc(sizeof(USER))) == NULL)
        {
            printf("memory error RightPassword");
            delay(3000);
            exit(1); //����ռ䲻�㣬�˳�����
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fread(u, sizeof(USER), 1, fp);
        for (j = 0; j < 13; j++)
        {
            if (u->UserName[j] != account[j])
            {
                break;
            }
            if (account[j] == '\0')
            {
                k = 1; //���ڴ��˻�
                break;
            }
        }
        if (k == 1)
        {
            for (j = 0; j < 13; j++)
            {
                if (u->password[j] != key[j])
                {
                    flag = 0;
                    break;
                }
                if (key[j] == '\0')
                {
                    flag = 1;
                    break;
                }
            }
            break;
        }
        free(u);
        u = NULL;
    }
    if (u != NULL)
    {
        free(u);
    }

    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file UserInfo.");
        delay(3000);
        exit(1);
    }
    return flag;
}

/*�������ܣ��ж��û��Ƿ񱻶��� 
  ����ֵ��1��ʾ���ᣬ0��ʾδ����*/
char judge_userfrozen(char account[])
{
    FILE *fp;
    USER *u = NULL;
    int i, j, k = 0;
    char flag = 0;
    int length;
    if ((fp = fopen("UserInfo.dat", "rb+")) == NULL)
    {
        printf("\nUserInfo.dat error file");
        delay(3000);
        exit(1); //��ʧ�ܣ��˳�����
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) / sizeof(USER);
    for (i = 0; i < length; i++)
    {
        if ((u = (USER *)malloc(sizeof(USER))) == NULL)
        {
            printf("memory error RightPassword");
            delay(3000);
            exit(1); //����ռ䲻�㣬�˳�����
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fread(u, sizeof(USER), 1, fp);
        for (j = 0; j < 13; j++)
        {
            if (u->UserName[j] != account[j])
            {
                break;
            }
            if (account[j] == '\0')
            {
                k = 1; //���ڴ��˻�
                break;
            }
        }
        if (k == 1)
        {
            if (u->UserState == '1')
            {
                flag = 0;
            }
            else
            {
                flag = 1;
            }
            break;
        }
        free(u);
        u = NULL;
    }
    if (u != NULL)
    {
        free(u);
    }

    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file UserInfo.");
        delay(3000);
        exit(1);
    }
    return flag;
}

/*�������ܣ����ʻ�����
����ֵ��0�ʻ��ѱ����� 1���ʻ�����ɹ� 2���ʻ�������*/
int user_freeze(char account[])
{
    FILE *fp;
    USER *u = NULL;
    int i, j, k = 0;
    int length;
    int flag;
    if ((fp = fopen("UserInfo.dat", "rb+")) == NULL)
    {
        printf("\nUserInfo.dat error file");
        delay(3000);
        exit(1); //��ʧ�ܣ��˳�����
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) / sizeof(USER);
    for (i = 0; i < length; i++)
    {
        k = 0;
        if ((u = (USER *)malloc(sizeof(USER))) == NULL)
        {
            printf("memory error RightPassword");
            delay(3000);
            exit(1); //����ռ䲻�㣬�˳�����
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fread(u, sizeof(USER), 1, fp);
        for (j = 0; j < 12; j++)
        {
            if (u->UserName[j] != account[j])
            {
                break;
            }
            if (account[j] == '\0')
            {
                k = 1; //���ڴ��˻�
                break;
            }
        }
        if (k == 1)
        {
            if (u->UserState == '2')
            {
                flag = 0;
            }
            if (u->UserState == '1')
            {
                flag = 1;
                u->UserState = '2';
            }
        }
        else if (flag != 0 && flag != 1)
        {
            flag = 2;
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fwrite(u, sizeof(USER), 1, fp);
        free(u);
        u = NULL;
    }
    if (u != NULL)
    {
        free(u);
    }
    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file UserInfo.");
        delay(3000);
        exit(1);
    }
    return flag;
}

/*�������ܣ����ʻ��ⶳ
����ֵ��0�ʻ��ѱ��ⶳ 1���ʻ��ⶳ�ɹ� 2���ʻ�������*/
char user_unfreeze(char account[])
{
    FILE *fp;
    USER *u = NULL;
    int i, j, k = 0;
    int length;
    char flag;
    if ((fp = fopen("UserInfo.dat", "rb+")) == NULL)
    {
        printf("\nUserInfo.dat error file");
        delay(3000);
        exit(1); //��ʧ�ܣ��˳�����
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) / sizeof(USER);
    for (i = 0; i < length; i++)
    {
        k = 0;
        if ((u = (USER *)malloc(sizeof(USER))) == NULL)
        {
            printf("memory error RightPassword");
            delay(3000);
            exit(1); //����ռ䲻�㣬�˳�����
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fread(u, sizeof(USER), 1, fp);
        for (j = 0; j < 12; j++)
        {
            if (u->UserName[j] != account[j])
            {
                break;
            }
            if (account[j] == '\0')
            {
                k = 1; //���ڴ��˻�
                break;
            }
        }
        if (k == 1)
        {
            if (u->UserState == '1')
            {
                flag = 0;
            }
            if (u->UserState == '2')
            {
                flag = 1;
                u->UserState = '1';
            }
        }
        else if (flag != 0 && flag != 1)
        {
            flag = 2;
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fwrite(u, sizeof(USER), 1, fp);
        free(u);
        u = NULL;
    }
    if (u != NULL)
    {
        free(u);
    }
    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file UserInfo.");
        delay(3000);
        exit(1);
    }
    return flag;
}

/*�������ܣ��ж��˺�����֤���Ƿ�ƥ��
  ����ֵ��0��ʾ����ȷ��1��ʾ��ȷ*/
char right_id(char account[], char id[])
{
    FILE *fp;
    USER *u = NULL;
    int i, j, k = 0;
    int flag = 0;
    int length;
    if ((fp = fopen("UserInfo.dat", "rb+")) == NULL)
    {
        printf("\nUserInfo.dat error file");
        delay(3000);
        exit(1); //��ʧ�ܣ��˳�����
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) / sizeof(USER);
    for (i = 0; i < length; i++)
    {

        if ((u = (USER *)malloc(sizeof(USER))) == NULL)
        {
            printf("memory error RightPassword");
            delay(3000);
            exit(1); //����ռ䲻�㣬�˳�����
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fread(u, sizeof(USER), 1, fp);
        k = 0;
        for (j = 0; j < 13; j++)
        {
            if (u->UserName[j] != account[j])
            {
                break;
            }
            if (account[j] == '\0')
            {
                k = 1; //���ڴ��˻�
                break;
            }
        }
        if (k == 1)
        {
            for (j = 0; j < 19; j++)
            {
                if (u->IDnumber[j] != id[j])
                {
                    flag = 0;
                    break;
                }
                if (id[j] == '\0')
                {
                    flag = 1;
                    break;
                }
            }
            break;
        }
        free(u);
        u = NULL;
    }
    if (u != NULL)
    {
        free(u);
    }
    fclose(fp);
    return flag;
}

/*�������ܣ��ж��Ƿ��������� �������˻�ע�ᣩ
  ����ֵ��1��ʾȫ�����룬2��ʾ�˺�δ���룬3��ʾ����δ���룬4��ʾδȷ�����룬5��ʾ����֤��δ����*/
char complete_register(char account, char key1, char key2, char id)
{
    if (account == '\0')
    {
        return 2;
    }
    if (key1 == '\0')
    {
        return 3;
    }
    if (key2 == '\0')
    {
        return 4;
    }
    if (id == '\0')
    {
        return 5;
    }
    return 1;
}

/*�������ܣ��ж��Ƿ��������� �������˻���¼��
  ����ֵ��1��ʾȫ�����룬2��ʾ�˺�δ���룬3��ʾ����δ����*/
char complete_login(char account, char key)
{
    if (account == '\0')
    {
        return 2;
    }
    if (key == '\0')
    {
        return 3;
    }
    return 1;
}

/*�������ܣ��ж��Ƿ��������� �����ڹ���Ա��¼��
  ����ֵ��1��ʾȫ�����룬2��ʾ����δ����*/
char complete_admin(char key)
{
    if (key == '\0')
    {
        return 2;
    }
    return 1;
}

/*�������ܣ���ע����Ϣд���ļ�*/
void wr_user_inf(char account[], char key[], char id[])
{
    FILE *fp;
    USER *u;
    int i;
    if ((fp = fopen("UserInfo.dat", "rb+")) == NULL)
    {
        printf("\nUserInfo.dat error file");
        delay(3000);
        exit(1); //��ʧ�ܣ��˳�����
    }
    if ((u = (USER *)malloc(sizeof(USER))) == NULL)
    {
        printf("memory error WrUserInf");
        delay(3000);
        exit(1); //����ռ䲻�㣬�˳�����
    }
    for (i = 0; i < 13; i++)
    {
        u->UserName[i] = account[i];
    }
    for (i = 0; i < 13; i++)
    {
        u->password[i] = key[i];
    }
    for (i = 0; i < 19; i++)
    {
        u->IDnumber[i] = id[i];
    }
    //	for(i=0;i<4;i++)
    //	{
    //		u->reportnum[i]='0';
    //	}
    //	u->reportnum[4]='\0';
    u->UserState = '1';
    u->n[0] = '\r';
    u->n[1] = '\n';
    fseek(fp, 0, SEEK_END);
    fwrite(u, sizeof(USER), 1, fp);
    free(u);
    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file UserInfo.");
        delay(3000);
        exit(1);
    }
}

/*�������ܣ���������д���ļ�*/
void user_change_password(char account[], char key[])
{
    FILE *fp;
    USER *u = NULL;
    int i, j, k = 0;
    int length;
    int mimalength;
    if ((fp = fopen("UserInfo.dat", "rb+")) == NULL)
    {
        printf("\nUserInfo.dat error file");
        delay(3000);
        exit(1); //��ʧ�ܣ��˳�����
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) / sizeof(USER);
    for (i = 0; i < length; i++)
    {
        k = 0;
        if ((u = (USER *)malloc(sizeof(USER))) == NULL)
        {
            printf("memory error RightPassword");
            delay(3000);
            exit(1); //����ռ䲻�㣬�˳�����
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fread(u, sizeof(USER), 1, fp);
        for (j = 0; j < 12; j++)
        {
            if (u->UserName[j] != account[j])
            {
                break;
            }
            if (account[j] == '\0')
            {
                k = 1; //���ڴ��˻�
                break;
            }
        }
        if (k == 1)
        {
            for (j = 0; j < 13; j++)
            {
                if (key[j] == '\0')
                {
                    mimalength = j + 1;
                    break;
                }
            }
            for (j = 0; j < mimalength; j++)
            {
                u->password[j] = key[j];
            }
            for (j = mimalength; j < 13; j++)
            {
                u->password[j] = '\0';
            }
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fwrite(u, sizeof(USER), 1, fp);
        free(u);
        u = NULL;
    }
    if (u != NULL)
    {
        free(u);
    }
    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file UserInfo.");
        delay(3000);
        exit(1);
    }
}

/*�������ܣ������ε�¼���˺�д���ļ�*/
void wr_user(char account[])
{
    FILE *fp;
    if ((fp = fopen("usernow.dat", "rb+")) == NULL)
    {
        printf("usernow.dat file error\n");
        delay(3000);
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    fwrite(account, sizeof(char), 13, fp);
    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file usernow.");
        delay(3000);
        exit(1);
    }
}

/*�������ܣ������ε�¼���˺Ŷ���*/
void re_user(char account[])
{
    FILE *fp;
    if ((fp = fopen("usernow.dat", "rb+")) == NULL)
    {
        printf("usernow.dat file error\n");
        delay(3000);
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    fread(account, sizeof(char), 13, fp);
    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file usernow.");
        delay(3000);
        exit(1);
    }
}

/*�������ܣ��жϹ���Ա��¼ʱ�����Ƿ����
  ����ֵ��0��ʾ����ȷ��1��ʾ��ȷ*/
char admin_password(char key[])
{
    char adpassword[5 + 1] = "admin";
    int i;
    int flag = 1;
    for (i = 0; i < 5; i++)
    {
        if (key[i] != adpassword[i])
        {
            flag = 0;
            break;
        }
    }
    if (key[5] != '\0')
    {
        flag = 0;
    }
    return flag;
}

/*�������ܣ��ж�id�Ƿ����
  ����ֵ��0��ʾ���ڣ�1��ʾ������*/
char judge_same_id(char idnumber[], char account[])
{
    FILE *fp;
    USER *u = NULL;
    int i, j;
    int flag = 1;
    int length;
    if ((fp = fopen("UserInfo.dat", "rb+")) == NULL)
    {
        printf("\nUserInfo.dat error file");
        delay(3000);
        exit(1); //��ʧ�ܣ��˳�����
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) / sizeof(USER);
    for (i = 0; i < length; i++)
    {
        if ((u = (USER *)malloc(sizeof(USER))) == NULL)
        {
            printf("memory error JudgeSameName");
            delay(3000);
            exit(1); //����ռ䲻�㣬�˳�����
        }
        fseek(fp, i * sizeof(USER), SEEK_SET);
        fread(u, sizeof(USER), 1, fp);
        for (j = 0; j < 19; j++)
        {
            if (u->IDnumber[j] != idnumber[j])
            {
                break;
            }
            if (idnumber[j] == '\0')
            {
                j = 19;
                break;
            }
        }
        if (j == 19)
        {
            flag = 0;
            strcpy(account, u->UserName);
            break;
        }
        free(u);
        u = NULL;
    }
    if (u != NULL)
    {
        free(u);
        u = NULL;
    }
    if (fclose(fp) != 0) //�ر��ļ�
    {
        printf("\nError in closing file UserInfo.");
        delay(3000);
        exit(1);
    }
    return flag;
}