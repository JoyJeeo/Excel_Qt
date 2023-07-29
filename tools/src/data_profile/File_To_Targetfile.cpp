#include "tools/include/data_profile/File_To_Targetfile.h"

#include <sstream>
#include <algorithm>
#include <QCoreApplication>
#include <QFileDialog>
#include <strings.h>
#include <QDirIterator>
#include <QDir>


File_To_Targetfile::File_To_Targetfile()
{
    /*
        产生该对象时，就应该存在的对象的"环境"
        构造对象的工作环境
    */
    try {
        // 分析项目路径
        profile_pro_path();

    } catch (...) {
        qDebug() << "File_To_Targetfile::File_To_Targetfile";
        throw;
    }
}

const string File_To_Targetfile::total_task(const string& input_file_path,string output_file_name)
{
    /*
        功能：
            1. 执行这个封装类所需要执行的全部任务，一次批量完成
            2. 将输入文件处理后，生成目标文件，并将target_file的绝对路径进行返回
    */
    try {
        // 动态获取程序的输出文件路径
        QString qstr_outfile_path = profile_output_file_path(output_file_name);
        string str_outfile_path = qstring_to_string(qstr_outfile_path);
        set_output_file_path(str_outfile_path);

        // 动态获取输入文件的路径
        // 使用上层传入的文件路径进行操作
        set_input_file_path(input_file_path);

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

const string File_To_Targetfile::merge_task(const QStringList &file_paths)
{
    try {
        // 将多个单文件路径传入，解析合并出alls文件
        merge_files_solo_data(file_paths);

        // 合并后的文件，作为source_file，衔接上该类的总任务total_task
        return total_task(ALLS_MERGE_FILE_PATH,"target_file.csv");
    } catch (...) {
        qDebug() << "string File_To_Targetfile::merge_task";
        throw ;
    }
}

const string File_To_Targetfile::time_task(const QStringList &dir_paths)
{
    /*
        功能：
            将不同时刻的文件内容合并为一个文件，文件名为time，文件内容为多个No的数据，
            将他们分别生成为一个独立文件，存在一个指定的文件夹中
            【会生成很多的文件】
    */
    try {
        // 创建存储中间数据的文件夹
//        // 存储文件
//        QString time_files_path = build_time_dir();
//        // 每次都清空time文件夹内容
//        clear_time_dir(time_files_path);

        // 用来收集所有时刻文件的数据
        vector<vector<string>> time_datas;

        // 默认文件夹都存在
        // 一批目录，就是一个true，因为是合成为一个文件
        // 控制了获取哪个数据的头数据
        bool first_flage = true;
        for(int i = 0;i < dir_paths.size();i++)
        {
            // 处理单文件夹
            tackle_single_dir(dir_paths[i],time_datas,first_flage);
        }

        // 将数据内容进行保存
        // 创建时刻文件timc【做出源文件】
        QString time_file_path = profile_output_file_path(time_file_name.toStdString());
        string str_time_file_path = qstring_to_string(time_file_path);
        set_timc_file_path(str_time_file_path);
        // 将数据写入文件中
        ofstream ofs = output_file_open(TIME_FILE_PATH);
        // 将处理好的数据输出到target_file中
        save_tackle_datas(ofs,time_datas);
        ofs.close();

        // 调用total函数，生成翻转源文件的target_file文件【生成target_file】
        return total_task(TIME_FILE_PATH,"target_file.csv");


    } catch (...) {
        qDebug() << "File_To_Targetfile::time_task";
        throw;
    }
}

void File_To_Targetfile::tackle_single_dir(QString dir_path,vector<vector<string>>& time_datas,bool& flage)
{
    /*
        功能：

    */
    try {
        // 获取文件夹内的所有文件名称
        QDir dir(dir_path);
        // 保证文件夹一定存在，且文件夹内部没有文件夹
        // 只留下csv文件
        QStringList filter;
        filter << "*.csv";
        dir.setNameFilters(filter);
        // 提取目录内的文件信息生成列表
        QList<QFileInfo> file_info(dir.entryInfoList());

        // 获取文件夹中所有文件的绝对路径
        QStringList list;
        // 遍历文件夹内的所有csv文件
        // 将csv文件的路径存在容器中
        for(int i = 0;i < file_info.size();i++)
        {
//            qDebug() << file_info[i].fileName();
            list << file_info[i].filePath();
        }

        // 分析文件夹的名称，获取文件内数据所在的时刻
        QString time_dir_name = profile_time_file_name(dir_path.toStdString());
        // 获取处理好的数据
        time_file_merge(list,time_datas,flage,time_dir_name);

    } catch (...) {
        qDebug() << "File_To_Targetfile::tackle_single_dir";
        throw;
    }
}

void File_To_Targetfile::time_file_merge(const QStringList &file_paths,
                                         vector<vector<string>>& time_datas,bool& flage,
                                         QString time_name)
{
    /*
        说明：
            一个时刻文件下的多个no芯片，按照part进行标记，
            因此，每个时刻文件下，都会有这依次多的no芯片，进行相互比较
        功能：
            将一个时刻下的多个no芯片数据进行合并到统一的数据存储区中time_datas
    */
    try {
        // 一个文件夹下的内容的part，都是从1开始计算的
        size_t counter = 1;
        // 依次获取单文件路径
        // 填补merge_datas中的数据
        for(auto path = file_paths.begin();path != file_paths.end();path++,counter++)
        {
            // 转换单文件路径为string
            string t_path = (*path).toStdString();
            // 打开该单数据文件
            ifstream ifs = input_file_open(t_path);
            // 将单数据文件内容全部读入程序中
            vector<vector<string>> all_arrary = tackle_file_get_all(ifs);
            // 分析单文件的有效数据SITE_NUM的起始位置和数据的有效长宽
            profile_col_row_num(all_arrary);

            // 一般非第一个文件时，直接读取最后数据即可
            // 一般的单数据表，只用读取其中最后一行的数据即可
            size_t all_array_end = all_arrary.size() - 1;

            // 【数据获取】
            // 只有当flage为true时，才获取数据的头数据，否则只获取元数据
            if(!flage)
            {
                // 输入的数据需要补0，长度不够要求
                while(all_arrary[all_array_end].size() != cols_num) all_arrary[all_array_end].push_back(""); // 补0
                // 将SITE_NUM的内容改为其所在文件夹的hr名称
                all_arrary[all_array_end][0] = time_name.toStdString();
                // 将原数据的part_id与指定文件的part进行对应
                all_arrary[all_array_end][1] = to_string(counter);
                time_datas.push_back(all_arrary[all_array_end]); // 将结果直接插入
                continue;
            }

            // 第一个文件时，读取其表头数据作为所有数据的极限值
            for(size_t i = targe_data_index;i <= all_array_end;i++)
            {
                // 输入的数据需要补0，长度不够要求
                while(all_arrary[i].size() != cols_num) all_arrary[i].push_back(""); // 补0
                if(i == all_array_end)
                {
                    // 将SITE_NUM的内容改为其所在文件夹的hr名称
                    all_arrary[all_array_end][0] = time_name.toStdString();
                }
                time_datas.push_back(all_arrary[i]); // 将结果直接插入
            }
            // 很好的控制了数据的一次有效性 【必须在内部控制标记的变化，引用传入】
            flage = false;
        }


    } catch (...) {
        qDebug() << "File_To_Targetfile::time_file_merge";
        throw;
    }
}

QString File_To_Targetfile::profile_time_file_name(const string &dir_path)
{
    /*
        功能：
            获取文件夹中的有效时刻名称并返回
    */
    try {
        // 分析获取文件名称
        int len = dir_path.size();

        //遍历找到文件夹的"_"位置
        int _ = len - 1;
        for(; _ > 0 ; _--)
        {
            if(dir_path[_] == '_')break;
        }

        // 默认找到了_位置
        return QString::fromStdString(dir_path.substr(_ + 1,len - _));

    } catch (...) {
        qDebug() << "File_To_Targetfile::profile_time_file_name";
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
            if(all_array[i][0] == target_str)
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
            // SITE_NUM不一定为整数
            if(is_Integer_Numeric(target_arrays[row][col]) || is_Integer_Numeric(target_arrays[row+1][col]))
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

//        test_datas(ans);

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

        // test_datas(ans);

        return ans;

    } catch (...) {
        qDebug() << "File_To_Targetfile::tackle_file_get_ans";
        throw;
    }
}

void File_To_Targetfile::merge_files_solo_data (const QStringList& file_paths)
{
    /*
        功能：
            将多个单数据文件内容进行合并，合并为一个文件内容，使用第一个文件的头数据作为合并文件的指标数据,
            并将合并内容后的结果存储在alls.csv文件中
            【第一个文件内容的选取很重要】 【暂时情况】【！！！】
            【可以支持单文件读取，不支持多文件多数据读取合并】
    */
    try {
        vector<vector<string>> merge_datas;
        size_t counter = 1;
        // 依次获取单文件路径
        // 填补merge_datas中的数据
        for(auto path = file_paths.begin();path != file_paths.end();path++,counter++)
        {
            // 转换单文件路径为string
            string t_path = (*path).toStdString();
            // 打开该单数据文件
            ifstream ifs = input_file_open(t_path);
            // 将单数据文件内容全部读入程序中
            vector<vector<string>> all_arrary = tackle_file_get_all(ifs);
            // 分析单文件的有效数据SITE_NUM的起始位置和数据的有效长宽
            profile_col_row_num(all_arrary);

            // 一般非第一个文件时，直接读取最后数据即可
            // 一般的单数据表，只用读取其中最后一行的数据即可
            size_t all_array_end = all_arrary.size() - 1;
            if(counter != 1)
            {
                // 输入的数据需要补0，长度不够要求
                while(all_arrary[all_array_end].size() != cols_num) all_arrary[all_array_end].push_back(""); // 补0
                // 将原数据的part_id与指定文件的part进行对应
                all_arrary[all_array_end][1] = to_string(counter);
                merge_datas.push_back(all_arrary[all_array_end]); // 将结果直接插入
                continue;
            }

            // 第一个文件时，读取其表头数据作为所有数据的极限值
            for(size_t i = targe_data_index;i <= all_array_end;i++)
            {
                // 输入的数据需要补0，长度不够要求
                while(all_arrary[i].size() != cols_num) all_arrary[i].push_back(""); // 补0
                merge_datas.push_back(all_arrary[i]); // 将结果直接插入
            }
        }

        // 将合并好的文件生成在alls.csv中即可
        // 获取alls.csv的存放路径
        QString alls_file_path = profile_output_file_path("alls.csv");
        string str_alls_file_path = qstring_to_string(alls_file_path);
        set_alls_file_path(str_alls_file_path);
        // 打开alls.csv文件，将合并好的数据放入进行存储
        ofstream ofs = output_file_open(ALLS_MERGE_FILE_PATH);
        // 将处理好的数据输出到target_file中
        save_tackle_datas(ofs,merge_datas);
        ofs.close();

    } catch (...) {
        qDebug() << "File_To_Targetfile::merge_files_solo_data";
        throw;
    }
}

void File_To_Targetfile::update_data_part(string &data, int part)
{
    try {
        int fst_bot = find_str_tag_dex(data,',',1);
        int sec_bot = find_str_tag_dex(data,',',2);
        string pre_str = data.substr(0,fst_bot+1);
        string part_str = to_string(part);
        string last_str = data.substr(sec_bot,data.size()-sec_bot);

//        data = pre_str + part_str + last_str;

    } catch (...) {
        qDebug() << "File_To_Targetfile::update_data_part";
        throw ;
    }
}

int File_To_Targetfile::find_str_tag_dex(const string &data, const char &c, int count)
{
    try {
        size_t i = 0;
        for(;i < data.size();i++)
        {
            if(data[i] == c)
            {
                count--;
                if(count == 0)break;
            }
        }

        return i;

    } catch (...) {
        qDebug() << "File_To_Targetfile::find_str_tag_dex";
        throw;
    }
}

void File_To_Targetfile::profile_pro_path() noexcept
{
    pro_path = QCoreApplication::applicationDirPath();
}

const QString File_To_Targetfile::build_time_dir()
{
    /*
        功能：
            保证项目所在的文件内存在TIME文件夹，存储时刻的合并文件
        返回值：
            返回创建的该文件的位置
    */
    try {
        QString ans = pro_path + "/" + time_file_name;

        QDir pic_dir(ans);
        // 判断文件是否已经存在
        // 如果文件夹已经存在
        if(!pic_dir.exists())
        {
            pic_dir.mkdir(ans);
        }

        return ans;

    } catch (...) {
        qDebug() << "QString File_To_Targetfile::build_time_dir";
        throw;
    }
}

void File_To_Targetfile::clear_time_dir(const QString &time_dir)
{
    try {
        QDir dir(time_dir);
        if(dir.isEmpty()) return;

        // 清空目录下的所有内容
        // 第三个参数是QDir的过滤参数，这三个表示收集所有文件和目录，且不包含"."和".."目录。
        // 因为只需要遍历第一层即可，所以第四个参数填QDirIterator::NoIteratorFlags
        QDirIterator DirsIterator(time_dir, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
        while(DirsIterator.hasNext())
        {
            if (!dir.remove(DirsIterator.next())) // 删除文件操作如果返回否，那它就是目录
            {
                QDir(DirsIterator.filePath()).removeRecursively(); // 删除目录本身以及它下属所有的文件及目录
            }
        }
    } catch (...) {
        qDebug() << "File_To_Targetfile::clear_time_dir";
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

void File_To_Targetfile::set_alls_file_path(const string &path) noexcept
{
    ALLS_MERGE_FILE_PATH = path;
}

string File_To_Targetfile::get_alls_file_path() noexcept
{
    return ALLS_MERGE_FILE_PATH;
}

void File_To_Targetfile::set_timc_file_path(const string &path) noexcept
{
    TIME_FILE_PATH = path;
}

string File_To_Targetfile::get_timc_file_path() noexcept
{
    return TIME_FILE_PATH;
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

QString File_To_Targetfile::profile_output_file_path(string output_file_name)
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
        QString output_file_path = pro_path + "/" +
                            QString::fromStdString(output_file_name); // target_file.csv
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
















