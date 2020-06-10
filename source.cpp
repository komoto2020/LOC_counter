#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
void show(std::string file_name = "NoFile", std::string class_function_name = "all", int number_of_method = 0, int number_of_lines = 0);
void showError(int error_flag);
int main (int argc, char* argv[]) // function begin
{
	int error_flag = 0;
	if (argc < 2) //引数があるかどうか
	{
		error_flag = 1;
		showError(error_flag);
		exit(-1);
	}
	std::cout << "ファイル名\t" << "クラス・関数名\t" << "メソッド数\t" << "行数\t" << "合計行数\t" << std::endl;
	int argv_index = 1;
	while (argc > argv_index)
	{
		std::string file_name = argv[argv_index];
		int format_index = file_name.find_last_of(".");
		std::string format_name = file_name.substr(format_index, file_name.size() - format_index);
		if (format_name != ".c" && format_name != ".cpp") //ファイルの拡張子が .c または .cppかどうか
		{
			error_flag = 2;
			showError(error_flag);
			exit(-1);
		}
		std::ifstream reading_file(file_name, std::ios::in);
		if (reading_file.fail()) //ファイルが開けたか
		{
			error_flag = 3;
			showError(error_flag);
			exit(-1);
		}
		show(file_name); //ファイル名の表示
		std::string reading_line_buffer = "";
		int source_all_count = 0;
		int class_count = 0;
		int method_count = 0;
		int function_count = 0;
		std::string class_name = "";
		std::string function_name = "";
		bool isInClass = false;
		bool isInFunction = false;
		while (std::getline(reading_file, reading_line_buffer)) //ファイルから１行読み込めたか
		{
			if (reading_line_buffer == "" || reading_line_buffer.find("/*") == 0) //ブランク行またはコメントのみの行か
			{
				continue;
			}
			source_all_count++;
			if (isInClass)
			{
				class_count++;
				if (reading_line_buffer.find("class end") != std::string::npos) //目印class endがあるか
				{
					isInClass = false;
					show("NoFile", class_name, method_count, class_count);
					method_count = 0;
					class_count = 0;
					continue;
				}
				if (reading_line_buffer.find("method") != std::string::npos) //目印methodがある
				{
					method_count++;
				}
				continue;
			}
			if (reading_line_buffer.find("class begin") != std::string::npos) //目印class beginがある
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
				if (reading_line_buffer.find("function end") != std::string::npos) //目印function endがある
				{
					isInFunction = false;
					show("NoFile", function_name, 0, function_count);
					function_count = 0;
				}
				continue;
			}
			if (reading_line_buffer.find("function begin") != std::string::npos) //目印function beginがある
			{
				std::stringstream stringstream;
				stringstream.str(""); //stringstreamの初期化
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
	std::cout << "エラー番号";
	switch (error_flag)
	{
	case 1:
		std::cout << error_flag << "：少なくとも１つのファイルを指定してください" << std::endl;
		break;
	case 2:
		std::cout << error_flag << "：ファイルのフォーマットが異なります" << std::endl;
		break;
	case 3:
		std::cout << error_flag << "：ファイルが見つかりません" << std::endl;
		break;
	default:
		break;
	}
	return;
} // function end