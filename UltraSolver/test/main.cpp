#include <vector>
#include <filesystem>
#include <sstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

// 递归调用取子文件夹所有的文件
void getAllFiles(const fs::path str_path, vector<fs::path>& files) {

	for (auto& fe : fs::directory_iterator(str_path)) {
		const auto& fp = fe.path();
		auto fFilename = fp.filename();

		// 递归调用, .和..不取
		if (fs::is_directory(fe)) {
			if (fFilename != "." && fFilename != "..") {
				files.push_back(fp);
				getAllFiles(fp, files);
			}
		}
		else {
			files.push_back(fp);
		}
	}
}

// 只获取一层文件,文件夹不取
void get_files(fs::path strPath, vector<fs::path>& files) {
	// 文件只取path
	for (auto& fe : fs::directory_iterator(strPath))
		if (!fs::is_directory(fe.path()))
			files.push_back(fe.path());
}

void int2str(const int& int_temp, string & string_temp) {
	stringstream stream;
	stream << int_temp;
	string_temp = stream.str();   //此处也可以用 stream>>string_temp
}

template<typename T>
T str2number(const string & string_temp) {
	T res;
	stringstream stream(string_temp);
	stream >> res;
	return res;
}

int main(int argc, char** argv) {
	//数据文件路径
	const fs::path root = "E:\\X3Benchmarks";
	//    fs::path root_folder = "QRandom-mdd-7-25-5";
	const fs::path root_folder = "Random-B-2-23-23";
	fs::path root_path = root;
	//组合路径
	root_path /= root_folder;
	cout << "instance path: " << root_path << endl;
	// 获得文件路径
	vector<fs::path> files;
	// 所有文件与文件夹的路径都输出
	get_files(root_path, files);
	//排序文件路径,自带可用,很棒!
	sort(files.begin(), files.end());
	
	for (auto& f : files) {
		cout << f << endl;
	}
}