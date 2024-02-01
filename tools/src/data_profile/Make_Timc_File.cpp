#include "tools/include/data_profile/Make_Timc_File.h"

#include <QDebug>
#include <QCoreApplication>
#include <fstream>
#include <iostream>
#include <QDir>
#include <QMessageBox>
#include <sstream>

Make_Timc_File::Make_Timc_File()
{
    try {
        profile_output_file_path();

    } catch (...) {
        qDebug() << "Make_Timc_File::Make_Timc_File";
        throw;
    }
}

Make_Timc_File::~Make_Timc_File()
{

}

const string Make_Timc_File::make_timc_file(const QStringList &dir_paths)
{
    try {
        // 遍历处理所有文件夹
        bool first_file_flage = true; // 一个文件夹里可能有多个数据，因此必须引用传入在内部进行修改

        for(int i = 0;i < dir_paths.size();i++)
        {
            tackle_single_dir(dir_paths[i],first_file_flage);
        }

        // 保存数据
        save_datas();

        if(test_plan.warning_flage())
            test_plan.warning_show();
        if(test_plan.warning_extra_flage())
            test_plan.warning_extra_show();

        return timc_file_path.toStdString();

    } catch (...) {
        qDebug() << "Make_Timc_File::make_timc_file";
        throw;
    }
}

ifstream Make_Timc_File::input_file_open(const string &input_file_path)
{
    try {
        ifstream ifs;
        ifs.open(input_file_path);
        if(!ifs.is_open())
        {
            cerr<<"timc file open fail"<<endl;
            exit(1);
        }
        return ifs; // 被移交内存

    } catch (...) {
        qDebug() << "Make_Timc_File::input_file_open";
        throw;
    }
}

ofstream Make_Timc_File::output_file_open(const string &out_file_path)
{
    try {
        ofstream ofs;
        ofs.open(out_file_path,ios::out | ios::trunc);
        if(!ofs.is_open())
        {
            cerr<<"please close the timc.csv, open fail"<<endl;
            exit(1);
        }

        return ofs;

    } catch (...) {
        qDebug() << "Make_Timc_File::output_file_open";
        throw;
    }
}

void Make_Timc_File::tackle_single_dir(const QString &dir_path, bool &first_file_flage)
{
    try {
        // 获取文件夹的时刻名称
        string time_name = profile_dir_name(dir_path);

        // 获取文件夹中所需的文件信息
        QList<QFileInfo> file_infos = profile_dir_inner_file_infos(dir_path);

        // 获取文件夹内文件的绝对路径
        QStringList file_paths = profile_dir_inner_file_paths(file_infos);

        // 遍历所有文件的绝对路径并处理所有文件
        for(int i = 0;i < file_paths.size();i++)
        {
            tackle_single_file(time_name,file_paths[i],first_file_flage);
        }

    } catch (...) {
        qDebug() << "Make_Timc_File::tackle_single_dir";
        throw;
    }
}

void Make_Timc_File::tackle_single_file(const string &time_name, const QString &file_path,
                                        bool &first_file_flage)
{
    try {
        // 根据文件路径获取No号
        vector<string> no_name = profile_no_name(file_path);

        // 获取单文件的修改建议
        pair<vector<string>,string> steer = make_pair(no_name,time_name);

        // 获取文件的输入流
        ifstream ifs = input_file_open(file_path.toStdString());

        // 将文件内容全部读入程序中
        vector<vector<string>> all_array = tackle_solo_file_get_all(ifs);

        // 关闭读入流
        ifs.close();

        // 获取目标数据区参数
        size_t rows_num = 0;
        size_t cols_num = 0;
        size_t target_data_index = 0;

        // 分析all_array，获取目标数据区的参数
        profile_target_data_params(all_array,rows_num,cols_num,target_data_index);

        // 获取目标数据区数据
        vector<vector<string>> target_array = tackle_solo_file_get_target(all_array,rows_num,
                                                                 cols_num,target_data_index);

        // 获取目标数据区中的分割点index
        size_t end_head_begin_body = 0;

        // 分析目标数据的分割点下标，分析出头数据和体数据的位置
        profile_div_index(target_array,end_head_begin_body);

        // 头数据处理
        // 获取头数据
        vector<vector<string>> head_datas = get_head_datas(target_array,end_head_begin_body);
        test_plan.warning_head(head_datas,file_path.toStdString());
        // 变动率中，根据test plan获取的有效列的索引数据
        map<string,int> base_pro_col = profile_pro_col_by_testplan(head_datas,time_name,file_path);
        // 只有第一个文件会获取头数据
        if(first_file_flage)
        {
            update_head_datas_by_testplan(head_datas, base_pro_col);
            // 将头数据加载进总容器中
            load_datas(head_datas);

            // 引用获取first_file_flage【只处理一次头数据】【由于一个文件夹下处理多个文件，因此必须在这里将flage进行修改】
            // 保证唯一性
            first_file_flage = false;
        }

        // 体数据处理
        vector<vector<string>> body_datas = get_body_datas(target_array,end_head_begin_body);
        // 修改body_datas中的数据
        vector<vector<string>> update_body = update_body_datas(body_datas,steer);
        // 将boday_datas中的数据与test plan的项目进行对齐
        update_body_datas_by_testplan(update_body, base_pro_col);

        // 加载进容器中
        load_datas(update_body);


    } catch (...) {
        qDebug() << "Make_Timc_File::tackle_signle_file";
        throw;
    }
}

string Make_Timc_File::profile_dir_name(const QString &dir_path)
{
    /*
        说明：
            文件夹的命名规则：*_时间
    */
    try {
        // 获取文件夹的文件名
        string integral_dir_name =  profile_path_to_name(dir_path);

        // 获取文件名的下划线位置
        int underlline_index = profile_underline_index(integral_dir_name);
        // 除去'_'获得文件名中的时间
        underlline_index++;

        // 获取变动率时间
        string time_name = integral_dir_name.substr(underlline_index,
                                              integral_dir_name.size() - underlline_index);


        return time_name;

    } catch (...) {
        qDebug() << "Make_Timc_File::profile_dir_name";
        throw;
    }
}

//bool qsort_cmp(const QFileInfo& a,const QFileInfo& b)
//{
//    return a.path() < b.path();
//}

QList<QFileInfo> Make_Timc_File::profile_dir_inner_file_infos(const QString &dir_path,
                                                             const QString &need_files)
{
    /*
        说明：
            1. 根据变动率文件的要求分析，这里默认文件夹内部没再有文件夹，
                且文件夹一定存在【这点由上层函数保证】
            2. 默认之过滤获得.csv文件，但保留接口
    */
    try {
        // 获取文件夹对象
        QDir dir(dir_path);

        // 只获取文件夹内指定的文件类型的文件
        QStringList filter;
        filter << need_files; // filter
        dir.setNameFilters(filter);
//        dir.setSorting(QDir::Name);

        // 提取目录内的文件信息生成列表
        QList<QFileInfo> file_infos(dir.entryInfoList());

//        // 获取的文件信息，按照文件名进行排序
        sort(file_infos.begin(),file_infos.end(),[](const QFileInfo& a,const QFileInfo& b){
            // 排序时，文件名的长度大小应该作为判断的依据之一，否则容易出现排序混乱
            return (a.fileName().length() < b.fileName().length()) ||
                    (a.fileName().length() == b.fileName().length() && a.fileName() < b.fileName());
        });


        return file_infos;

    } catch (...) {
        qDebug() << "Make_Timc_File::profile_dir_inner_file_info";
        throw;
    }
}

QStringList Make_Timc_File::profile_dir_inner_file_paths(const QList<QFileInfo> &file_infos)
{
    try {
        // 获取文件夹中所有文件的绝对路径
        QStringList file_paths_list;

        for(int i = 0;i < file_infos.size();i++)
        {
            // 绝对路径
            file_paths_list << file_infos[i].filePath();
        }

        return file_paths_list;

    } catch (...) {
        qDebug() << "Make_Timc_File::profile_dir_inner_file_paths";
        throw;
    }
}

vector<string> Make_Timc_File::profile_no_name(const QString &file_path)
{
    /*
        说明：
            文件夹中文件名的命名规则：N01-N02.csv,N03-N04.csv,N05-N06.csv,...
            将N12.csv -> No_12进行返回
    */
    try {
        // 获取完整的文件名
        string integral_file_name = profile_path_to_name(file_path);

        // 将文件名作为输入端重新读入
        istringstream sin(integral_file_name);

        // 存储字符串段
        vector<string> str_names;
        string temp_str;

        // 将file_name以'_'为分界点，进行字符串内容读取
        while(getline(sin,temp_str,'-'))
        {
            // cout<<array.size()<<" ";
            str_names.push_back(temp_str);
        }

        // 【对字符串段进行分析】
        // 对分割的字符串进行分析
        int len = str_names.size();
        int last_index = len - 1;

        // 存储No的容器
        vector<string> nos;

        // 分析no
        for(int i = 0;i < last_index;i++)
        {
            string no = str_names[i];
            string ans_no = "No_" + no.substr(1,no.size() - 1);


            // 加入nos
            nos.push_back(ans_no);
        }

        // 分析末尾的特殊字符串
        string last_str = str_names[last_index];
        string last_no = "";
        for(size_t i = 0;i < last_str.size();i++)
        {
            if(last_str[i] == '_' || last_str[i] == '.')
            {
                // 做最后的处理
                nos.push_back(last_no);

                break;
            }

            // 获取no
            if(last_str[i] != 'N')
            {
                last_no += last_str[i];
            }
            // 遇到'N'
            else {
                last_no = "No_";
            }
        }

        return nos;

    } catch (...) {
        qDebug() << "Make_Timc_File::profile_file_name";
        throw;
    }
}

string Make_Timc_File::profile_path_to_name(const QString &path)
{
    /*
        说明：
            获取路径中最后一个'/'后的内容
    */
    try {
        string str_path = path.toStdString();

        size_t i;

        for(i = str_path.size() - 1;i >=0 ;i--)
        {
            if(str_path[i] == '/' || str_path[i] == '\\')break;
        }
        i++;

        string name = str_path.substr(i,str_path.size() - i);

        return name;

    } catch (...) {
        qDebug() << "Make_Timc_File::profile_path_to_name";
        throw;
    }
}

int Make_Timc_File::profile_underline_index(const string &name)
{
    try {
        size_t i = -1;
        for(i = name.size() - 1;i >= 0;i--)
        {
            if(name[i] == '_')break;
        }

        return i;

    } catch (...) {
        qDebug() << "Make_Timc_File::profile_underline_index";
        throw;
    }
}

int Make_Timc_File::profile_nameDot_index(const string &name)
{
    try {
        size_t i = -1;
        for(i = name.size() - 1;i >= 0;i--)
        {
            if(name[i] == '.')break;
        }

        return i;

    } catch (...) {
        qDebug() << "Make_Timc_File::profile_nameDot_index";
        throw;
    }
}

vector<vector<string>> Make_Timc_File::tackle_solo_file_get_all(const ifstream &ifs)
{
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
        qDebug() << "Make_Timc_File::tackle_solo_file_get_all";
        throw;
    }
}

void Make_Timc_File::profile_target_data_params(vector<vector<string>> &all_array,
                                                size_t &rows_num, size_t &cols_num,
                                                size_t &target_data_index)
{
    try {
        // 记录数据有效的标记
        bool valid_row_flage = false;
        // 遍历分析目标数据的行列数
        // 判断是否找到了目标点
        bool target_index_flage = false;
        for(size_t i = 0; i < all_array.size(); i++)
        {
            // 跳过无效的空行
            if(all_array[i].size() == 0 || all_array[i][0].size() == 0)continue;
            // 找到目标数据的起点
            // 非理想情况，则判断后跳回重新判断理想位置【必须位于target_str判断你的前方，进行i值和数据的修正】
            if(all_array[i][0] == "Unit" && !target_index_flage) // "Unit"点
            {
                // 进来这里，说明前面没找到目标点，则对目标点做重新修改后，重新判断
                i -= 1;
                all_array[i][0] = target_str;
            }
            else if(all_array[i][0] == "LimitL" && !target_index_flage) // "LimitL"点
            {
                // 进来这里，说明前面没找到目标点，则对目标点做重新修改后，重新判断
                i -= 2;
                all_array[i][0] = target_str;
            }
            else if(all_array[i][0] == "LimitU" && !target_index_flage) // "LimitU"点
            {
                // 进来这里，说明前面没找到目标点，则对目标点做重新修改后，重新判断
                i -= 3;
                all_array[i][0] = target_str;
            }

            // 理想情况，找到目标的起点位置 【顺序不能变】
            if(all_array[i][0] == target_str)
            {
                // 如果找到目标，记录开始位置
                target_data_index = i;
                // 目标位置的字符串个数，即cols_num的有效列数
                cols_num = all_array[i].size();
                // 设置从此刻开始，记录有效行的个数
                valid_row_flage = true;
                target_index_flage = true;
            }

            // 如果满足flage，则rows_num++
            if(valid_row_flage)
            {
                rows_num++;
            }
        }

    } catch (...) {
        qDebug() << "Make_Timc_File::profile_target_data_params";
        throw;
    }
}

vector<vector<string>> Make_Timc_File::tackle_solo_file_get_target(vector<vector<string>> &all_array,
                                                                    size_t rows_num, size_t cols_num,
                                                                    size_t target_data_index)
{
    try {
        vector<vector<string>> target_arrays;

        // 经过profile_col_row_num优化后的目标获取的循环
        // 获取目标数据
        for(size_t i = target_data_index;i < all_array.size();i++)
        {
            // 输入的数据需要补0，长度不够要求
            while(all_array[i].size() != cols_num) all_array[i].push_back(""); // 补0
            target_arrays.push_back(all_array[i]); // 将结果直接插入
        }

        // test_datas(target_arrays);

        return target_arrays;

    } catch (...) {
        qDebug() << "Make_Timc_File::tackle_solo_file_get_target";
        throw;
    }
}

void Make_Timc_File::profile_div_index(const vector<vector<string>> &target_datas,
                                       size_t &end_head_begin_body)
{
    try {
        size_t& i = end_head_begin_body;
        for(i = 0;i < target_datas.size();i++)
        {
            if(target_datas[i][0] == this->div_str || target_datas[i].size() == 0) break;
        }

        // 空行的下一位为数据起始位，保证结尾为开区间'[)'
        i++;

    } catch (...) {
        qDebug() << "Make_Timc_File::profile_div_index";
        throw;
    }
}

vector<vector<string>> Make_Timc_File::get_head_datas(const vector<vector<string>> &target_datas,
                                                       size_t end_head_begin_body)
{
    try {
        vector<vector<string>> head_datas;

        for(size_t i = 0;i < end_head_begin_body;i++)
        {
            head_datas.push_back(target_datas[i]);
        }

        return head_datas;

    } catch (...) {
        qDebug() << "Make_Timc_File::get_head_datas";
        throw;
    }
}

vector<vector<string>> Make_Timc_File::get_body_datas(const vector<vector<string>> &target_datas,
                                                       size_t end_head_begin_body)
{
    try {
        vector<vector<string>> body_datas;

        for(size_t i = end_head_begin_body;i < target_datas.size();i++)
        {
            body_datas.push_back(target_datas[i]);
        }

        return body_datas;

    } catch (...) {
        qDebug() << "Make_Timc_File::get_body_datas";
        throw;
    }
}

vector<vector<string>> Make_Timc_File::update_body_datas(const vector<vector<string>> &body_datas,
                                                          const pair<vector<string>,string>& steers)
{
    /*
        说明：
            1. 如果一个芯片在一个时刻内测试时，获取了多条part数据，则以其最后一次获取的数据为
                该芯片在该时刻内获取的准确数据的依据
            2. steers中，first为No_...的数据，second为每个时刻的名称Pre,Pst,...
    */
    try {
        // 创建二维数组，为了匹配load_datas的数据加载功能
        vector<vector<string>> update_body;

        for(size_t i = 0;i < steers.first.size();i++)
        {
            update_body.push_back(body_datas[i]);
            // 修改SITE_NUM
            // 一般将文件名中的芯片编号N01...，改为No_01,No_02,...
            update_body[i][0] = steers.first[i];

            // 修改PART_ID
            update_body[i][1] = steers.second;

        }

        return update_body;

    } catch (...) {
        qDebug() << "Make_Timc_File::update_body_datas";
        throw;
    }
}

map<string, int> Make_Timc_File::profile_pro_col_by_testplan(const vector<vector<string>> &head_datas,
                                                             const string &time_name, const QString &file_path)
{
    try {
        map<string, int> base_pro_col;
        map<string, Unit_UL_Str> pro_set = test_plan.get_test_plan_map();

        // 扫原始的head项目，找对应pro_set填充base_pro_index
        // 获取有效列的起始位置
        size_t begin_row = 0;
        size_t col_end_invalid_begin_pro;

        for(col_end_invalid_begin_pro = 0; col_end_invalid_begin_pro < head_datas[begin_row].size();col_end_invalid_begin_pro++)
        {
            if(head_datas[begin_row][col_end_invalid_begin_pro] == "TEST_NUM")
            {
                col_end_invalid_begin_pro++;
                break;
            }
        }
        // 开始扫所有项目，完成base_pro_index
        for(size_t col = col_end_invalid_begin_pro;col < head_datas[begin_row].size();col++)
        {
            if(pro_set.find(head_datas[begin_row][col]) != pro_set.end())
            {
                base_pro_col.insert(make_pair(head_datas[begin_row][col], col));
            }
        }

        // Warning 变动率中，多出的项目不要，少的项目报错并说明少哪些项目
        // 将test plan中的所有项目都先加入non中
        set<string> non_pro;
        for(auto& item : pro_set) non_pro.insert(item.first);
        // 将base_pro_col中与non_pro一致的项目都删除
        for(auto& item : base_pro_col) non_pro.erase(item.first);
        // 正常non应该为空，否则报错
        if(non_pro.size() != 0)
        {
            string warning_str = "Timc: " + time_name + "\n" +
                    "File_Path: " + file_path.toStdString() + "\n" +
                    "[Non_Pros]: \n";
            int counter = 1;
            for(auto& item : non_pro)
            {
                warning_str += item + (counter % 4 != 0 ? ',' : '\n');
                counter++;
            }
            warning_str = warning_str.substr(0,warning_str.size() - 1);
            QMessageBox::warning(nullptr,"timc test_plan error",QString::fromStdString(warning_str));
            exit(1);
        }

        return base_pro_col;
    } catch (...) {
        qDebug() << "Make_Timc_File::profile_pro_col_by_testplan";
        throw;
    }
}

void Make_Timc_File::update_head_datas_by_testplan(vector<vector<string> > &head_datas, map<string,int>& base_pro_col)
{
    try {
        vector<string> pro_list = test_plan.get_test_plan_pro_list();

        // 获取有效列的起始位置
        size_t begin_row = 0;
        size_t col_end_invalid_begin_pro;
        size_t end_row = 5;

        for(col_end_invalid_begin_pro = 0; col_end_invalid_begin_pro < head_datas[begin_row].size();col_end_invalid_begin_pro++)
        {
            if(head_datas[begin_row][col_end_invalid_begin_pro] == "TEST_NUM")
            {
                col_end_invalid_begin_pro++;
                break;
            }
        }

        // 将原始head_datas的数据重新填充进res
        vector<vector<string>> res(end_row, vector<string>());
        for(size_t col = 0;col < col_end_invalid_begin_pro;col++)
        {
            for(size_t row = 0;row < end_row;row++)
                res[row].push_back(head_datas[row][col]);
        }
        for(size_t i = 0;i < pro_list.size();i++)
        {
            assert(base_pro_col.find(pro_list[i]) != base_pro_col.end());
            int col = base_pro_col[pro_list[i]];

            for(size_t row = 0;row < end_row;row++)
                res[row].push_back(head_datas[row][col]);
        }

        // 最后将res与head_dats数据进行交换
        head_datas = res;

    } catch (...) {
        qDebug() << "Make_Timc_File::update_head_datas_by_testplan";
        throw;
    }
}

void Make_Timc_File::update_body_datas_by_testplan(vector<vector<string> > &body_datas, map<string,int>& base_pro_col)
{
    try {
        vector<string> pro_list = test_plan.get_test_plan_pro_list();

        // 获取有效列的起始位置
        size_t begin_row = 0;
        size_t col_end_invalid_begin_pro = 6; // 手动设置
        size_t end_row = body_datas.size();

        // 将原始body_datas数据对应base_pro_index重新填充res
        vector<vector<string>> res(end_row, vector<string>());
        for(size_t col = 0;col < col_end_invalid_begin_pro;col++)
        {
            for(size_t row = 0;row < end_row;row++)
                res[row].push_back(body_datas[row][col]);
        }
        for(size_t i = 0;i < pro_list.size();i++)
        {
            assert(base_pro_col.find(pro_list[i]) != base_pro_col.end());
            int col = base_pro_col[pro_list[i]];

            for(size_t row = 0;row < end_row;row++)
                res[row].push_back(body_datas[row][col]);
        }

        // res的数据即为body_datas所需的最终数据，二者交换
        body_datas = res;

    } catch (...) {
        qDebug() << "Make_Timc_File::update_body_datas_by_testplan";
        throw;
    }
}

void Make_Timc_File::load_datas(const vector<vector<string>> &datas)
{
    try {
        for(size_t i = 0;i < datas.size();i++)
        {
            time_datas.push_back(datas[i]);
        }

    } catch (...) {
        qDebug() << "Make_Timc_File::load_datas";
        throw;
    }
}

void Make_Timc_File::profile_output_file_path() noexcept
{
    timc_file_path = QCoreApplication::applicationDirPath() +
            "/" + timc_file_name;
}

void Make_Timc_File::save_datas()
{
    try {
        // 获取文件输出流
        ofstream ofs = output_file_open(timc_file_path.toStdString());

        // 将时刻数据内容写出
        for(size_t i = 0;i < time_datas.size(); i++)
        {
            for(size_t j = 0;j < time_datas[i].size();j++)
            {
                ofs<<time_datas[i][j]<<",";
            }
            ofs<<endl;
        }

        ofs.close();

    } catch (...) {
        qDebug() << "Make_Timc_File::save_datas";
        throw;
    }
}

void Make_Timc_File::test_datas(const vector<vector<string> > &arrays)
{
    try {
        for(size_t i = 0;i < arrays.size(); i++)
        {
            for(size_t j = 0;j < arrays[i].size();j++)
            {
                cout<<arrays[i][j]<<" ";
            }
            cout<<endl;
        }

    } catch (...) {
        qDebug() << "Make_Timc_File::test_datas";
        throw;
    }
}
