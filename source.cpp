#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
void show(std::string file_name = "NoFile", std::string class_function_name = "all", int number_of_method = 0, int number_of_lines = 0);
void showError(int error_flag);
int main (int argc, char* argv[]) // function begin
{
	int error_flag = 0;
	if (argc < 2) //���������邩�ǂ���
	{
		error_flag = 1;
		showError(error_flag);
		exit(-1);
	}
	std::cout << "�t�@�C����\t" << "�N���X�E�֐���\t" << "���\�b�h��\t" << "�s��\t" << "���v�s��\t" << std::endl;
	int argv_index = 1;
	while (argc > argv_index)
	{
		std::string file_name = argv[argv_index];
		int format_index = file_name.find_last_of(".");
		std::string format_name = file_name.substr(format_index, file_name.size() - format_index);
		if (format_name != ".c" && format_name != ".cpp") //�t�@�C���̊g���q�� .c �܂��� .cpp���ǂ���
		{
			error_flag = 2;
			showError(error_flag);
			exit(-1);
		}
		std::ifstream reading_file(file_name, std::ios::in);
		if (reading_file.fail()) //�t�@�C�����J������
		{
			error_flag = 3;
			showError(error_flag);
			exit(-1);
		}
		show(file_name); //�t�@�C�����̕\��
		std::string reading_line_buffer = "";
		int source_all_count = 0;
		int class_count = 0;
		int method_count = 0;
		int function_count = 0;
		std::string class_name = "";
		std::string function_name = "";
		bool isInClass = false;
		bool isInFunction = false;
		while (std::getline(reading_file, reading_line_buffer)) //�t�@�C������P�s�ǂݍ��߂���
		{
			if (reading_line_buffer == "" || reading_line_buffer.find("/*") == 0) //�u�����N�s�܂��̓R�����g�݂̂̍s��
			{
				continue;
			}
			source_all_count++;
			if (isInClass)
			{
				class_count++;
				if (reading_line_buffer.find("class end") != std::string::npos) //�ڈ�class end�����邩
				{
					isInClass = false;
					show("NoFile", class_name, method_count, class_count);
					method_count = 0;
					class_count = 0;
					continue;
				}
				if (reading_line_buffer.find("method") != std::string::npos) //�ڈ�method������
				{
					method_count++;
				}
				continue;
			}
			if (reading_line_buffer.find("class begin") != std::string::npos) //�ڈ�class begin������
			{
				std::stringstream stringstream;
				stringstream.str("");
				stringstream << reading_line_buffer;
				stringstream >> class_name >> class_name;
				isInClass = true;
				class_count++;
				continue;
			}
			if (isInFunction)
			{
				function_count++;
				if (reading_line_buffer.find("function end") != std::string::npos) //�ڈ�function end������
				{
					isInFunction = false;
					show("NoFile", function_name, 0, function_count);
					function_count = 0;
				}
				continue;
			}
			if (reading_line_buffer.find("function begin") != std::string::npos) //�ڈ�function begin������
			{
				std::stringstream stringstream;
				stringstream.str(""); //stringstream�̏�����
				stringstream << reading_line_buffer;
				stringstream >> function_name >> function_name;
				isInFunction = true;
				function_count++;
			}
		}
		argv_index++;
	}
	return 0;
} // function end 
void show (std::string file_name, std::string class_function_name, int number_of_method, int number_of_lines) // function begin
{
	if (file_name != "NoFile")
	{
		std::cout << file_name << std::endl;
	}
	else if (class_function_name == "all")
	{
		for (int i = 0; i < 8; i++)
		{
			std::cout << "\t";
		}
		std::cout << number_of_lines << std::endl;
	}
	else
	{
		std::cout << "\t\t" << class_function_name << "\t" << number_of_method << "\t" << number_of_lines << std::endl;
	}
	return;
} // function end
void showError (int error_flag) // function begin
{
	std::cout << "�G���[�ԍ�";
	switch (error_flag)
	{
	case 1:
		std::cout << error_flag << "�F���Ȃ��Ƃ��P�̃t�@�C�����w�肵�Ă�������" << std::endl;
		break;
	case 2:
		std::cout << error_flag << "�F�t�@�C���̃t�H�[�}�b�g���قȂ�܂�" << std::endl;
		break;
	case 3:
		std::cout << error_flag << "�F�t�@�C����������܂���" << std::endl;
		break;
	default:
		break;
	}
	return;
} // function end