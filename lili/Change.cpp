#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

vector<double> meter_times;
vector<string> meter_str;

int feet_spe = 0;


/*记录输入串中的各个元素*/
struct _ele {
	/*基数*/
	double base;
	/*单位对应的倍数*/
	double times;
};

vector<string> splitString(const string& src, string separate_character)   
{   
	vector<string> strs;   

	int separate_characterLen = separate_character.size(); 
	int lastPosition = 0,index = -1;   
	while (string::npos != (index = src.find(separate_character,lastPosition)))   
	{   
		strs.push_back(src.substr(lastPosition,index - lastPosition));   
		lastPosition = index + separate_characterLen;   
	}   
	string lastString = src.substr(lastPosition);//截取最后一个分隔符后的内容   
	if (!lastString.empty())   
		strs.push_back(lastString);//如果最后一个分隔符后还有内容就入队   
	return strs;   
}   

int init(char* filename)
{
	ifstream in_file (filename);
	if (!in_file.is_open())
	{
		return -1;
	}
	string line;
	while (getline(in_file,line))
	{
		if (line.find("=") != string::npos)
		{
			vector<string> fields = splitString(line," ");
			if(fields[1] == "foot")
			{
				feet_spe = meter_str.size();
			}
			meter_str.push_back(fields[1]);
			meter_times.push_back(atof(fields[3].c_str()));
		}
	}
	in_file.close();
	return 0;
}

/*
* input:带转换的字符串，如" - 1.2"
* return:字符串对应的值
*/
double getBase (char* str)
{
	if (str == NULL|| strlen(str) == 0)
		return 0;
	char* num = new char[strlen(str)];
	int i = 0;
	while (*str != '\0')
	{
		if (*str != ' ' && *str != '+')
		{
			num[i++] = *str;
		}
		str++;
	}
	num[i] = '\0';
	double ret = atof(num);
	delete[] num;
	return ret;
}

vector<_ele> parse(char* str)
{
	vector<_ele> dest;
	if (str == NULL || strlen(str) == 0)
	{
		return dest;
	}
	char* data = str;
	char* tmp = NULL;
	char* end = data + strlen(str);
	while (*data != '\0')
	{
		char* old = end;
		int match = -1;
		for (int i = 0;i < meter_str.size();i++)
		{
			tmp = strstr(data,meter_str[i].c_str());			
			if (tmp < old && tmp != NULL)
			{
				old = tmp;
				match = i;
			}
			else
			{
				tmp = old;
			}
		}
		/*foot的复数形式特殊处理*/
		tmp = strstr(data,"feet");
		if (tmp != NULL && tmp < old)
		{
			match = feet_spe;
		}
		else
		{
			tmp = old;
		}
		
		if (tmp != NULL)
		{
			_ele element;
			element.times = meter_times[match];
			*tmp = '\0';
			element.base = getBase(data);
			data = tmp + meter_str[match].length();
			dest.push_back(element);
		}
		/*跳过复数部分*/
		while(*data != '\0')
		{
			if (*data == '+' || *data == '-')
			{
				break;
			}
			data++;
		}
	}
	return dest;
}

void test(char* in_filename,char* out_filename)
{
	ifstream in_file (in_filename);
	if (!in_file.is_open())
	{
		return ;
	}
	ofstream out_file(out_filename);
	if (!out_file.is_open())
	{
		return;
	}
	out_file << "lili2012@software.ict.ac.cn" << endl;
	out_file << endl;
	string line;
	char out_format[128];
	while (getline(in_file,line))
	{
		if (line.find("=") == string::npos && line.length() > 0)
		{
			char* data = new char[line.length() + 1];
			data[0] = '\0';
			strncpy(data,line.c_str(),line.length());
			data[line.length()] = '\0';
			vector<_ele> elements = parse(data);
			double ret = 0;
			for (int i = 0;i < elements.size();i++)
			{
				ret += elements[i].base*elements[i].times;
			}	
			sprintf(out_format,"%.2f",ret);
			out_file << out_format << " m" << endl;
			delete[] data;
		}
	}
	in_file.close();
	out_file.close();
}

int main(int argc, char* argv[])
{
	if (init("input.txt") != -1)
	{
		test("input.txt","output.txt");
	}

	return 0;
}

