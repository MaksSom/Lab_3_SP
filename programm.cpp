#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <windows.h>

using namespace std;

enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Yellow = 6,
	White = 7,
	Gray = 8,
	Purple = 9,
	Orange = 10,
};

void setConsoleColor(ConsoleColor color) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
}

bool isIn(string a, vector < string> Constants)
{
	for (int i = 0; i < Constants.size(); i++)
	{
		if (a == Constants[i])
		{
			return true;
		}
	}
	return false;
}

bool isNumber(const string& s) {
	try {
		size_t pos;
		stoi(s, &pos);
		return pos == s.length();
	}
	catch (const invalid_argument&) {
		return false;
	}
	catch (const out_of_range&) {
		return false;
	}
}

bool isValidName(const std::string& input) {

	std::regex namePattern("^[a-zA-Z_][a-zA-Z0-9_-]*$");
	return std::regex_match(input, namePattern);
}

vector <pair <string, string > > FunctionDefiner(vector <pair <string, string > > Code)
{
	vector < string > Constants;
	vector < string > beforeIndent = {
		"var", "let", "const", "function", "class", "void"
	};
	vector < string > ReservedWords = {
		"break", "case", "catch",
		"continue", "debugger", "default", "delete", "do",
		"else", "export", "extends", "false", "finally",
		"for", "function", "if", "import", "in",
		"instanceof", "new", "null", "return", "super",
		"switch", "this", "throw", "true", "try",
		"typeof", "while", "with", "yield", "console", "log"
	};
	vector < string > Operators = {
		"+", "-", "*", "/", "%", "++", "--",
		"&&", "||", "!",
		"==", "===", "!=", ">", "<", ">=", "<=", "!==",
		"=", "+=", "-=", "*=", "/="
	};

	vector < string > Punctuation = {
		"(", ")", ",", ".", "[", "]", ":",
	};

	
	for(int i = 0; i < Code.size(); i++)
	{
		/*cout << "Working with: " << Code[i].first << " second: " << ".\n";
		for (int j = 0; j < Code.size(); j++)
		{
			cout << "\"" << Code[j].first << "\" \"" << Code[j].second << "\"" << endl;
		}
		cout << endl;*/

		if (i >= 1)
		{
			if (Code[i - 1].second == "//")
				if (Code[i].first != "\n")
				{
					Code[i].second = "//";
					continue;
				}

			if (Code[i - 1].second == "/*")
			{
				if (Code[i].first.find("*/") == std::string::npos)
					Code[i].second = "/*";
				else
				{
					int t = Code[i].first.find("*/");
					string word1 = "";
					for (int j = 0; j < t; j++)
					{
						word1 += Code[i].first[j];
					}
					word1 += "*/";

					string word2 = "";
					for (int j = t + 2; j < Code[i].first.size(); j++)
					{
						word2 += Code[i].first[j];
					}
					Code[i].first = word1;
					Code[i].second = "comment";
					if (i + 1 < Code.size())
						Code.insert(Code.begin() + i + 1, { word2, "" });
					else
						Code.push_back({ word2, "" });
				}
				continue;
			}

			if (Code[i - 1].second == "reserved_Variable")
			{
				//cout << "checking " << Code[i].first << endl;
				if (isValidName(Code[i].first))
				{
					Constants.push_back(Code[i].first);
					Code[i].second = "var";
				}
			}


			if (Code[i - 1].second == "charB" and Code[i].second != "charB")
			{
				if (Code[i].first.find("\'") == std::string::npos)
					if (Code[i].first == "\n")
					{
						Code[i].second = "char";
						continue;
					}

					int t = Code[i].first.find("\'");
					string word1 = "";
					for (int j = 0; j < t; j++)
					{
						word1 += Code[i].first[j];
					}
					word1 += "\'";

					string word2 = "";
					for (int j = t + 1; j < Code[i].first.size(); j++)
					{
						word2 += Code[i].first[j];
					}
					Code[i].first = word1;
					Code[i].second = "char";
					if (word2 != "")
						if (i + 1 < Code.size())
							Code.insert(Code.begin() + i + 1, { word2, "" });
						else
							Code.push_back({ word2, "" });
				continue;
			}

			if (Code[i - 1].second == "stringB" and Code[i].second != "stringB")
			{
				if (Code[i].first.find("\"") == std::string::npos)
					if (Code[i].first == "\n")
					{
						Code[i].second = "string";
						continue;
					}

					int t = Code[i].first.find("\"");
					string word1 = "";
					for (int j = 0; j < t; j++)
					{
						word1 += Code[i].first[j];
					}
					word1 += "\"";

					string word2 = "";
					for (int j = t + 1; j < Code[i].first.size(); j++)
					{
						word2 += Code[i].first[j];
					}
					Code[i].first = word1;
					Code[i].second = "string";
					if (word2 != "")
						if (i + 1 < Code.size())
							Code.insert(Code.begin() + i + 1, { word2, "" });
						else
							Code.push_back({ word2, "" });
					
				continue;
			}
		}
		if (Code[i].second == "")
		{
			if (Code[i].first.find("/*") != std::string::npos)
			{
				int t = Code[i].first.find("/*");
				string word1 = "";
				for (int j = 0; j < t; j++)
				{
					word1 += Code[i].first[j];
				}

				string word2 = "";
				for (int j = t + 2; j < Code[i].first.size(); j++)
				{
					word2 += Code[i].first[j];
				}
				word2 = "/*" + word2;
				Code[i].first = word1;
				Code[i].second = "";
				if (i + 1 < Code.size())
					Code.insert(Code.begin() + i + 1, { word2, "/*" });
				else
					Code.push_back({ word2, "" });
			}

			if (Code[i].first.find("//") != std::string::npos)
			{
				int t = Code[i].first.find("//");
				string word1 = "";
				for (int j = 0; j < t; j++)
				{
					word1 += Code[i].first[j];
				}

				string word2 = "";
				for (int j = t + 2; j < Code[i].first.size(); j++)
				{
					word2 += Code[i].first[j];
				}
				word2 = "//" + word2;

				if (t == 0)
				{
					Code[i].second = "//";
					continue;
				}

				Code[i].first = word1;
				Code[i].second = "";
				if (i + 1 < Code.size())
					Code.insert(Code.begin() + i + 1, { word2, "//" });
				else
					Code.push_back({ word2, "" });
			}

			if (Code[i].first.find("\'") != std::string::npos)
			{
				int t = Code[i].first.find("\'");
				string word1 = "";
				for (int j = 0; j < t; j++)
				{
					word1 += Code[i].first[j];
				}

				string word2 = "";
				for (int j = t + 2; j < Code[i].first.size(); j++)
				{
					word2 += Code[i].first[j];
				}
				word2 = "\'" + word2;

				if (t == 0)
				{
					Code[i].second = "charB";
					continue;
				}

				Code[i].first = word1;
				Code[i].second = "";
				if (i + 1 < Code.size())
					Code.insert(Code.begin() + i + 1, { word2, "charB" });
				else
					Code.push_back({ word2, "charB" });
			}

			if (Code[i].first.find("\"") != std::string::npos)
			{
				int t = Code[i].first.find("\"");
				string word1 = "";
				for (int j = 0; j < t; j++)
				{
					word1 += Code[i].first[j];
				}

				string word2 = "";
				for (int j = t + 2; j < Code[i].first.size(); j++)
				{
					word2 += Code[i].first[j];
				}
				word2 = "\"" + word2;

				if (t == 0)
				{
					Code[i].second = "stringB";
					continue;
				}

				Code[i].first = word1;
				Code[i].second = "";
				if (i + 1 < Code.size())
					Code.insert(Code.begin() + i + 1, { word2, "stringB" });
				else
					Code.push_back({ word2, "stringB" });
			}

			if (Code[i].first[Code[i].first.size() - 1] == ';')
			{
				string word1 = "";
				for (int j = 0; j < Code[i].first.size() - 1; j++)
					word1 += Code[i].first[j];
				Code[i].first = word1;
				if (i + 1 < Code.size())
					Code.insert(Code.begin() + i + 1, { ";", "punct" });
				else
					Code.push_back({ ";", "punct" });
			}

			for (int j = 0; j < Operators.size(); j++)
			{
				if (Code[i].first.find(Operators[j]) != std::string::npos)
				{
					string word1 = "";
					int temp = Code[i].first.find(Operators[j]);
					

					string word2 = "";
					for (int k = temp + Operators[j].size(); k < Code[i].first.size(); k++)
						word2 += Code[i].first[k];


					if (temp > 0)
					{
						for (int k = 0; k < temp; k++)
							word1 += Code[i].first[k];

						Code[i].first = word1;

						if (i + 1 < Code.size())
							Code.insert(Code.begin() + i + 1, { Operators[j], "operator" });
						else
							Code.push_back({ Operators[j], "operator" });

						if (word2 != "")
							if (i + 2 < Code.size())
								Code.insert(Code.begin() + i + 2, { word2, "" });
							else
								Code.push_back({ word2, "" });
					}
					else
					{
						for (int k = 0; k < Operators[j].size(); k++)
							word1 += Code[i].first[k];
						Code[i].first = word1;
						Code[i].second = "operator";
					}

					if(temp == 0)
						if (word2 != "")
							if (i + 1 < Code.size())
								Code.insert(Code.begin() + i + 1, { word2, "" });
							else
								Code.push_back({ word2, "" });
						
				}
			}

			for (int j = 0; j < Punctuation.size(); j++)
			{
				if (Code[i].first.find(Punctuation[j]) != std::string::npos)
				{
					string word1 = "";
					int temp = Code[i].first.find(Punctuation[j]);
					

					string word2 = "";
					for (int k = temp + Punctuation[j].size(); k < Code[i].first.size(); k++)
						word2 += Code[i].first[k];

					if(temp > 0)
					{
						for (int k = 0; k < temp; k++)
							word1 += Code[i].first[k];
						Code[i].first = word1;

						if (i + 1 < Code.size())
							Code.insert(Code.begin() + i + 1, { Punctuation[j], "punct" });
						else
							Code.push_back({ Punctuation[j], "punct" });

						if (word2 != "")
							if (i + 2 < Code.size())
								Code.insert(Code.begin() + i + 2, { word2, "" });
							else
								Code.push_back({ word2, "" });
					}
					else
					{
						for (int k = 0; k < Punctuation[j].size(); k++)
							word1 += Code[i].first[k];
						Code[i].first = word1;
						Code[i].second = "punct";
					}

					if (temp == 0)
						if (word2 != "")
							if (i + 1 < Code.size())
								Code.insert(Code.begin() + i + 1, { word2, "" });
							else
								Code.push_back({ word2, "" });
				}
			}


			if (isNumber(Code[i].first))
			{
				Code[i].second = "num";
			}

			/*if (Code[i].first[0] == '\'' and Code[i].first[Code[i].first.size() - 1] == '\'')
				if (Code[i].first.size() >= 2)
					Code[i].second = "char";

			if (Code[i].first[0] == '"' and Code[i].first[Code[i].first.size() - 1] == '"')
				if (Code[i].first.size() >= 2)
					Code[i].second = "string";*/

			if (isIn(Code[i].first, ReservedWords))
				Code[i].second = "reserved";

			if (isIn(Code[i].first, beforeIndent))
			{
				Code[i].second = "reserved_Variable";
			}

			if (isIn(Code[i].first, Constants))
				Code[i].second = "const";
		}
	}

	return Code;
}

int main()
{

	ifstream FILE;

	FILE.open("test.txt");

	if (!FILE.is_open())
	{
		cout << "File can't be opened" << endl;
		return 1;
	}

	string a = "";

	vector <pair <string, string > > Code;

	while (getline(FILE, a))
	{

		string word = "";
		for (char b : a)
		{
			if (b != ' ')
			{
				word = word + b;
			}
			else
			{
				if (word != "")
				{
					Code.push_back({ word, "" });
					word = "";
				}
				if (b == '\n')
				{
					Code.push_back({ word, "" });
					Code.push_back({ "\n", "" });
					word = "";
				}
			}
		}

		if (word != "")
			Code.push_back({ word, "" });

		Code.push_back({ "\n", "" });
	}

	

	cout << "numbers - Green" << endl
		<< "string and character constants - Purple" << endl
		<< "preprocessor directives - none" << endl
		<< "comments - Gray" << endl
		<< "reserved words - Magenta" << endl
		<< "operators - Yellow" << endl
		<< "punctuation - Cyan" << endl
		<< "identifiers - Red" << endl;

	cout << endl<< "Code:" << endl;
	Code = FunctionDefiner(Code);

	/*for (int i = 0; i < Code.size(); i++)
	{
		cout << "\"" << Code[i].first << "\" \"" << Code[i].second << "\"" << endl;
	}
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;*/

	for (int i = 0; i < Code.size(); i++)
	{
		if(Code[i].second == "comment" or Code[i].second == "//" or Code[i].second == "/*")
			setConsoleColor(Gray);
		if (Code[i].second == "var")
			setConsoleColor(Blue);
		if (Code[i].second == "charB" or Code[i].second == "stringB" or Code[i].second == "char" or Code[i].second == "string")
			setConsoleColor(Purple);
		if (Code[i].second == "num")
			setConsoleColor(Green);
		if (Code[i].second == "reserved" or Code[i].second == "reserved_Variable")
			setConsoleColor(Magenta);
		if (Code[i].second == "const")
			setConsoleColor(Red);
		if (Code[i].second == "punct")
			setConsoleColor(Cyan);
		if (Code[i].second == "operator")
			setConsoleColor(Yellow);
		cout << Code[i].first << " ";
		setConsoleColor(White);
	}
	cout << endl;

	return 0;
}
