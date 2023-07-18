#include "../../include/data_profile/File_To_Targetfile.h"

#include <sstream>
#include <algorithm>
#include <QCoreApplication>
#include <QFileDialog>
#include <strings.h>

const string File_To_Targetfile::total_task()
{
    /*
        功能：
            1. 执行这个封装类所需要执行的全部任务，一次批量完成
            2. 将输入文件处理后，生成目标文件，并将target_file的绝对路径进行返回
    */
    try {
        // 动态获取程序的输出文件路径
        QString qstr_outfile_path = profile_output_file_path();
        string str_outfile_path = qstring_to_string(qstr_outfile_path);
        set_output_file_path(str_outfile_path);
        // 动态获取输入文件的路径
        QString qstr_infile_path = profile_input_file_path();
        string str_infile_path = qstring_to_string(qstr_infile_path);
        // 如果没有输入文件
        if(str_infile_path == ""){
            return str_infile_path;
        }
        set_input_file_path(str_infile_path);

        // 打开输入文件
        ifstream ifs = input_file_open(IN_FILE_PATH);
        // 将该文件进行处理后将目标数据读入程序
        vector<vector<string>> datas = tackle_file(ifs);
        ifs.close();

        // 打开输出的target_file
        ofstream ofs = output_file_open(OUT_FILE_PATH);
        // 将处理好的数据输出到target_file中
        save_tackle_datas(ofs,datas);
        ofs.close();

        // 返回target_file的路径
        return OUT_FILE_PATH;

    } catch (...) {
        qDebug() << "File_To_Targetfile::total_task";
        throw;
    }
}

ifstream File_To_Targetfile::input_file_open(const string& input_File_path)
{
    /*打开输入文件*/
    try {
        ifstream ifs;
        ifs.open(input_File_path);
        if(!ifs.is_open())
        {
            cerr<<"source file open fail"<<endl;
            exit(1);
        }
        return ifs;

    } catch (...) {
        qDebug() << "File_To_Targetfile::input_file_open";
        throw;
    }
}

ofstream File_To_Targetfile::output_file_open(const string& output_File_path)
{
    /*打开输出文件*/
    try {
        ofstream ofs;
        ofs.open(output_File_path,ios::out | ios::trunc);
        if(!ofs.is_open())
        {
            cerr<<"please close the target_file.csv, open fail"<<endl;
            exit(1);
        }

        return ofs;

    } catch (...) {
        qDebug() << "File_To_Targetfile::output_file_open";
        throw;
    }
}

vector<vector<string>>
File_To_Targetfile::tackle_file(const ifstream& ifs)
{
    /*
        从输入文件的内容中，获取和筛选需要的数据，暂时存入容器中进行返回
        可以使用test_datas将返回的矩阵数据进行打印测试
    */
    try {
        vector<vector<string>> ans;
        vector<vector<string>> all_array = tackle_file_get_all(ifs);
        profile_col_row_num(all_array); // 处理获取目标数据的相关参数
        vector<vector<string>> target_arrays = tackle_file_get_target(all_array);
        vector<vector<string>> rows_array = tackle_file_get_rows(target_arrays);
        vector<vector<string>> cols_array = tackle_file_get_cols(target_arrays);
        ans = tackle_file_get_ans(rows_array,cols_array);

        return ans;

    } catch (...) {
        qDebug() << "File_To_Targetfile::tackle_file";
        throw;
    }
}

vector<vector<string>>
File_To_Targetfile::tackle_file_get_all(const ifstream& ifs)
{
    /*将整个xlsx表格读入程序中*/
    try {
        string s;
        vector<vector<string>> all_array;
        vector<string> arrays;
        string array;
        ifstream& t_ifs = const_cast<ifstream&>(ifs);
        while(getline(t_ifs,s))
        {
            istringstream sin(s);

            while(getline(sin,array,','))
            {
                // cout<<array.size()<<" ";
                arrays.push_back(array);
            }
            // cout<<endl;
            all_array.push_back(arrays);
            arrays.clear();
        }
//        test_datas(all_array);
        return all_array;

    } catch (...) {
        qDebug() << "File_To_Targetfile::tackle_file_get_all";
        throw;
    }
}

void File_To_Targetfile::profile_col_row_num(const vector<vector<string> > &all_array)
{
    /*
        参数：
            all_array：将数据源文件加载进程序后获得的数组
        功能：
            记录目标数据的起始位置：targe_data_index
            记录目标数据的长宽大小：rows_num,cols_num
            更新维护File的内部属性
    */
    try {
        // 初始化目标数据的行列数
        rows_num = 0,cols_num = 0,targe_data_index = 0;
        // 记录数据有效的标记
        bool flage = false;
        // 遍历分析目标数据的行列数
        for(size_t i = 0; i < all_array.size(); i++)
        {
            // 跳过无效的空行
            if(all_array[i].size() == 0)continue;
            // 找到目标数据的起点
            if(all_array[i][0] == "SITE_NUM")
            {
                // 如果找到目标，记录开始位置
                targe_data_index = i;
                // 目标位置的字符串个数，即cols_num的有效列数
                cols_num = all_array[i].size();
                // 设置从此刻开始，记录有效行的个数
                flage = true;
            }

            // 如果满足flage，则rows_num++
            if(flage)
            {
                rows_num++;
            }
        }

    } catch (...) {
        qDebug() << "File_To_Targetfile::profile_col_row_num";
        throw;
    }
}

vector<vector<string>>
File_To_Targetfile::tackle_file_get_target(const vector<vector<string>>& all_array)
{
    /*获取目标表格的数据载入程序中*/
    try {
        vector<vector<string>> target_arrays;
        // 去const，因为输入的数据可能会有一些问题
        vector<vector<string>>& t_all_array = const_cast<vector<vector<string>>&>(all_array);

        // cout<<all_array.size()<<" "<<all_array[0].size()<<endl;

        // 经过profile_col_row_num优化后的目标获取的循环
        // 获取目标数据
        for(size_t i = targe_data_index;i < t_all_array.size();i++)
        {
            // 输入的数据需要补0，长度不够要求
            while(t_all_array[i].size() != cols_num) t_all_array[i].push_back(""); // 补0
            target_arrays.push_back(t_all_array[i]); // 将结果直接插入
        }

//        bool flage = false; // 标记有效数据区开始的位置
//        for(size_t i = 0;i < t_all_array.size();i++)
//        {
//            if(t_all_array[i].size() != cols_num)continue;
////            if(t_all_array[i][0] == "SITE_NUM" )flage = true;
////            else flage = true;
//            else {
//                for(size_t j = i;j < t_all_array.size();j++)
//                {
//                    // 输入的数据需要补0，长度不够要求
//                    while(t_all_array[j].size() != cols_num) t_all_array[j].push_back(""); // 补0
//                    target_arrays.push_back(t_all_array[j]); // 将结果直接插入
//                }
//                break;
//            }
//        }

        // for(int i = 0;i < target_arrays.size();i++)
        // {
        //     for(int j = 0;j < target_arrays[i].size();j++)
        //     {
        //         cout<<target_arrays[i][j].size()<<" ";
        //     }
        //     cout<<endl;
        // }

        // test_datas(target_arrays);

        return target_arrays;

    } catch (...) {
        qDebug() << "File_To_Targetfile::tackle_file_get_target";
        throw;
    }
}

vector<vector<string>>
File_To_Targetfile::tackle_file_get_rows(const vector<vector<string>>& target_arrays)
{
    /*将目标数据以行形式存储*/
    try {
        vector<vector<string>> rows_array = target_arrays;
        // cout<<target_arrays[4][44].size()<<endl;

        // test_datas(target_arrays);

        return rows_array;

    } catch (...) {
        qDebug() << "File_To_Targetfile::tackle_file_get_rows";
        throw;
    }
}

vector<vector<string>>
File_To_Targetfile::tackle_file_get_cols(const vector<vector<string>>& target_arrays)
{
    /*将目标数据以列形式存储*/
    try {
        int col_number = target_arrays[0].size();
        int row_number = target_arrays.size();
        vector<vector<string>> cols_array(col_number,vector<string>()); // 每一行应该是target的每一列的数据

        // cout<<target_arrays.size()<<" "<<target_arrays[0].size()<<endl;
        // cout<<col_number<<" "<<row_number<<endl;
        for(int i = 0;i < col_number;i++)
        {
            for(int j = 0;j < row_number;j++)
            {
                // cout<<i<<":"<<j<<" ";
                // cout<<target_arrays[j][i]<<" ";
                // cout<<"1 ";
                // cout<<target_arrays[j].size()<<endl;
                cols_array[i].push_back(target_arrays[j][i]);
            }
            // cout<<endl;
        }
        // cout<<target_arrays[4][43].size()<<endl;
        // cout<<"ok"<<endl;

        // test_datas(cols_array);

        return cols_array;

    } catch (...) {
        qDebug() << "File_To_Targetfile::tackle_file_get_cols";
        throw;
    }
}

// map<string,vector<string>> tackle_file_get_maps(const vector<vector<string>>& cols_array)
// {
//    /*用于将vector数据进行map转换，暂时不需要使用*/
//     map<string,vector<string>> m;
//     for(int i = 0;i < cols_array.size(); i++)
//     {
//         m.insert(make_pair(cols_array[i][0],vector<string>(cols_array[i].begin()+1,cols_array[i].end())));
//     }
//     return m;
// }

size_t File_To_Targetfile::get_vec_col_index_valid(const vector<vector<string>>& target_arrays)
{
    /*
        根据SITE_NUM中的整数数据，来作为有效列索引的起点位置，
            因为SITE_NUM的有效数值一定是int，并且与其他属性的列数据一一对应，因此适合作为有效列起点的参考标志

        返回值为SITE_NUM所在第0行中第一个为int的数据的col索引
    */
    try {
        size_t row; // SITE_NUM所在行
        size_t col;
        for(row = 0,col = 0;col < target_arrays[row].size();col++)
        {
            if(is_Integer_Numeric(target_arrays[row][col]))
                break;
        }
        return col;

    } catch (...) {
        qDebug() << "File_To_Targetfile::get_vec_col_index_valid";
        throw;
    }
}

vector<vector<string>>
File_To_Targetfile::tackle_file_get_ans(const vector<vector<string>>& rows_array,
                                        const vector<vector<string>>& cols_array)
{
    /*
        参数：
            rows_array：行数据
            cols_array：列属性的数据数组
        功能：
            1. 以列属性的数据作为输出数据
            2. 将表项的属性数据进行进一步处理后输出
    */
    try {
        // 将列数据作为输出到targe_file的数据【以列为处理方式，更加方便数据的处理】
        vector<vector<string>> ans = cols_array;
        // map<string,vector<string>> m = tackle_file_get_maps(cols_array);
//        for(auto x : cols_array){
//            if(find(labels.begin(),labels.end(),x[0]) != labels.end())
//            {
//                ans.push_back(x);
//            }
//        }

//        test_datas(cols_array);

        // 遍历数据区，所有为空的位置，都用NULL直接填充即可
        //【符合要求，PASSFG为TRUE时，依然可能会有属性的数据为空，画图会崩溃，必须填充NULL】
        size_t row_len = ans.size();
        size_t col_len = ans[0].size();
        // 获取数据区开始的起始位置
        size_t begin_col = get_vec_col_index_valid(ans);
        for(size_t i = 0;i < row_len;i++)
        {
            for(size_t j = begin_col;j < col_len;j++)
            {
                if(ans[i][j].size() == 0)
                {
                    ans[i][j] = "NULL";
                }
            }
        }


        // 第一层循环，为了找到PASSFG的横坐标下标位置
//        for(size_t i = 0;i < cols_array.size(); i++)
//        {
////            qDebug() << "i: "<< i ;
//            if(cols_array[i][0] == "PASSFG")
//            {
//                // 第二层循环，为了找到FALSE的纵坐标下标位置
//                for(size_t j = 0;j < cols_array[i].size();j++)
//                {
////                    qDebug() << (QString::fromStdString(cols_array[i][j]));
//                    // 由于字符字符读入后可能被转换了大小，这里使用忽略大小写的方式进行处理
//                    if(str_cmp_ignore_case(cols_array[i][j],"FALSE")) // i已经是一个定值了
//                    {
////                        qDebug() << "i: " << i << "j: "<<j;
//                        // 第三层循环，为主循环，为了将标记为FALSE的所有数据中，为空的标记为NULL【这样的处理方式，在理论上更加严谨】
//                        for(size_t k = 2;k < ans.size();k++)
//                        {
//                            if(ans[k][j].size() == 0) ans[k][j] = "NULL";
//                        }
//                    }
//                }
//                break;
//            }
//        }

        // test_datas(ans);

        return ans;

    } catch (...) {
        qDebug() << "File_To_Targetfile::tackle_file_get_ans";
        throw;
    }
}

void File_To_Targetfile::save_tackle_datas(const ofstream& ofs,const vector<vector<string>>& datas)
{
    /*获得输出文件，将处理筛选过的数据写入输入文件中，作为中间文件进行使用*/
    try {
        ofstream& t_ofs = const_cast<ofstream&>(ofs);
        for(size_t i = 0;i < datas.size(); i++)
        {
            for(size_t j = 0;j < datas[i].size();j++)
            {
                t_ofs<<datas[i][j]<<",";
            }
            t_ofs<<endl;
        }

    } catch (...) {
        qDebug() << "File_To_Targetfile::save_tackle_datas";
        throw;
    }
}

bool File_To_Targetfile::str_cmp_ignore_case(const string &a, const string &b) noexcept
{
    /*
        功能：
            将a和b进行忽略字符串大小写的比较
    */
    try {
        // 如果长度都不相同，则无需比较
        if(a.size() != b.size())return false;

        // 获取长度，加速比较
        size_t len = a.size();
        // 遍历进行比较
        for(size_t i = 0;i < len;i++)
        {
            // 忽略大小写处理
            if(tolower(a[i]) != tolower(b[i]))return false;
        }

        return true;

    } catch (...) {
        qDebug() << "File_To_Targetfile::str_cmp_nocase";
        throw;
    }
}

void File_To_Targetfile::set_output_file_path(const string &path) noexcept
{
    OUT_FILE_PATH = path;
}

string File_To_Targetfile::get_output_file_path() noexcept
{
    return OUT_FILE_PATH;
}

void File_To_Targetfile::set_input_file_path(const string &path) noexcept
{
    IN_FILE_PATH = path;
}

string File_To_Targetfile::get_input_file_path() noexcept
{
    return IN_FILE_PATH;
}


// test
void File_To_Targetfile::test_datas(const vector<vector<string>>& arrays)
{
    // 展示处理的结果 test
    try {
        for(size_t i = 0;i < arrays.size(); i++)
        {
//            qDebug() << "i: " << i <<"arrays[i].size()" << arrays[i].size();
            for(size_t j = 0;j < arrays[i].size();j++)
            {
                cout<<arrays[i][j]<<" ";
            }
            cout<<endl;
        }

    } catch (...) {
        qDebug() << "File_To_Targetfile::test_datas";
        throw;
    }
}

bool File_To_Targetfile::is_Integer_Numeric(const string &str)
{
    /*
        通过判断str中是否有非0-9的数据，来判断str是否为一个存储数值的字符串

        该函数只能判断，str是否为一个非小数的数值数据
    */
    try {
        auto it = str.begin();
        while (it != str.end() && std::isdigit(*it)) {
            it++;
        }
        return !str.empty() && it == str.end();

    } catch (...) {
        qDebug() << "File_To_Targetfile::is_Integer_Numeric";
        throw;
    }
}

QString File_To_Targetfile::profile_input_file_path()
{
    /*
        使用文件对话框的形式，可视化的选择需要处理的.csv文件作为程序的输入文件
    */
    try {
        QString input_file_path = QFileDialog::getOpenFileName(Q_NULLPTR, // 不设置父窗体，独立窗体显示dialog即可
                                                       QObject::tr("select open file"), // 设置窗体标题
                                                       QObject::tr("../"), // 是指初始打开文件的位置
                                                       QObject::tr("File(*.csv);;All(*.*)")); // 设置可以筛选的文件类型
//        qDebug() << input_file_path;
        return input_file_path;

    } catch (...) {
        qDebug() << "File_To_Targetfile::profile_input_file_path";
        throw;
    }
}

QString File_To_Targetfile::profile_output_file_path(QString output_file_name)
{
    /*
        函数功能：
            1. 获取.exe文件所在的目录的绝对路径，并拼凑上生成的目标文件的文件名，将目标文件生成在.exe所在的目录中
            2. 使用默认参数方式实现，可以修改输出文件的文件名

        函数产生的原因：
            1. 由于项目文件可能存在于不同的位置，因此通过计算的方式获取输出文件的绝对路径【灵活性】
            2. 输出文件的路径，又会作为valid_data获取数据是读取的文件路径，qt读取文件必须是绝对路径【读入路径必须为绝对路径】

    */
    try {
        QString output_file_path = QCoreApplication::applicationDirPath() +
                                        QObject::tr("\\") + output_file_name; // target_file.csv
        return output_file_path;

    } catch (...) {
        qDebug() << "File_To_Targetfile::profile_output_file_path";
        throw;
    }
}

string File_To_Targetfile::qstring_to_string(const QString &qstr)
{
    /*使用函数方式，将qstring转换为string*/
    try {
        return qstr.toStdString();

    } catch (...) {
        qDebug() << "File_To_Targetfile::qstring_to_string";
        throw;
    }
}
















