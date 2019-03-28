#include <vector>
#include <filesystem>
#include <sstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

// �ݹ����ȡ���ļ������е��ļ�
void getAllFiles(const fs::path str_path, vector<fs::path>& files) {

	for (auto& fe : fs::directory_iterator(str_path)) {
		const auto& fp = fe.path();
		auto fFilename = fp.filename();

		// �ݹ����, .��..��ȡ
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

// ֻ��ȡһ���ļ�,�ļ��в�ȡ
void get_files(fs::path strPath, vector<fs::path>& files) {
	// �ļ�ֻȡpath
	for (auto& fe : fs::directory_iterator(strPath))
		if (!fs::is_directory(fe.path()))
			files.push_back(fe.path());
}

void int2str(const int& int_temp, string & string_temp) {
	stringstream stream;
	stream << int_temp;
	string_temp = stream.str();   //�˴�Ҳ������ stream>>string_temp
}

template<typename T>
T str2number(const string & string_temp) {
	T res;
	stringstream stream(string_temp);
	stream >> res;
	return res;
}

int main(int argc, char** argv) {
	//�����ļ�·��
	const fs::path root = "E:\\X3Benchmarks";
	//    fs::path root_folder = "QRandom-mdd-7-25-5";
	const fs::path root_folder = "Random-B-2-23-23";
	fs::path root_path = root;
	//���·��
	root_path /= root_folder;
	cout << "instance path: " << root_path << endl;
	// ����ļ�·��
	vector<fs::path> files;
	// �����ļ����ļ��е�·�������
	get_files(root_path, files);
	//�����ļ�·��,�Դ�����,�ܰ�!
	sort(files.begin(), files.end());
	
	for (auto& f : files) {
		cout << f << endl;
	}
}