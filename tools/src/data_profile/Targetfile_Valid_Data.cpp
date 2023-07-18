#include "../../include/data_profile/Targetfile_Valid_Data.h"

#include <fstream>
#include <sstream>
using namespace std;

void Targetfile_Valid_Data::profile_get_site_parts()
{
    /*
        数据的起点位置：
            获取SITE_NUM 和 PART_ID
            默认为列表的第一行 和 第二行的数据
        存储：
            存储数据在m_site_parts.m_site_part当中，其为一个map<string,vector<int>>结构
        说明：
            1. 行索引上，扫描SITE_NUM,PART_ID，他们都一定处于0，1索引位置
            2. 列索引上，从出现第一个整数的部分开始扫描获取数据
        功能：
            获取site,part 对应的数据
    */
    try {
        for(size_t i = 0;i < 2;i++)
        {
            string attri;
            for(size_t j = 0;j < m_source_target_file_vec[i].size();j++)
            {
                // 获取对应属性名称
                if(j == 0) attri = m_source_target_file_vec[i][j];
                // 从出现第一个整数数据处开始记录
                if(is_Integer_Numeric(m_source_target_file_vec[i][j]))
                {
                    // 获取 attri -> Integer
                    m_site_parts.m_site_part[attri].push_back(stoi(m_source_target_file_vec[i][j]));
                }
            }
        }

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::profile_get_site_parts";
        throw;
    }
}

void Targetfile_Valid_Data::profile_get_attri_uuls()
{
    /*
        数据的起点位置：
            1. 使用默认参数，normal_attri = "TEST_NUM"，
                定义uul获取数据的起始attri的前一位属性
                【因为前一位TEST_NUM可能是一个固定不变的末尾属性】【这样方便作为两组数据的的拆分】
            2. 默认情况下以TEST_NUM为两组数据拆分的分界点，使用默认参数留住接口位置，为后续可能的修改做准备

        数据的范围：
            Unit:[已定义单位,空]
            [LimitL,LimitU]:[INT_MIN,INT_MAX]

        存储内容：
            三个属性对应的vector<double>中，都会跳过数据表中为"空"的区域
            Unit:
                存储已定义的单位
                未定义的单位为"空"，即字符串长度为0，允许被存储
            LimitL:
                存储已定义的LimitL
                未定义的LimitL，以INT_MIN进行存储
            LimitU:
                存储已定义的LimitU
                未定义的LimitU，以INT_MAX进行存储
        存储：
            Unit,LimitL,LimitU存储在m_attri_uuls.m_attri_uuls中，
                其结构为 map<string,Attri_UL_Unit_NUMBER::UL_Unit_NUMBER>
                实现 attri -> uul的对应
        功能：
            从有效数据行开始依次搜索，获取列中小于end_col_dex【实际是有效数据区的起始列下标】，以此作为列的结束标记，
            获取每行attri -> uul
    */

    try {
        // 获取起点获取数据的位置
        size_t begin_row_dex = get_source_vec_row_index_by_attri(div_attri) + 1;
        size_t end_col_dex = get_source_vec_col_index_valid();

        for(size_t i = begin_row_dex;i < m_source_target_file_vec.size();i++)
        {
            string attri=m_source_target_file_vec[i][0]; // 存储属性名称
            string unit=""; // 初始化，有些属性可能并没有单位【这是合法的】
            double limitL=INT_MIN,limitU=INT_MAX; // 需要插入的data数据

            // 对列依次查找,到end_col_dex分界点时结束遍历
            for(size_t j = 0;j < end_col_dex;j++)
            {
                // unit定义,未定义都存储
                if(m_source_target_file_vec[0][j] == m_attri_uuls.m_Unit)
                    unit = m_source_target_file_vec[i][j];
                // limitL 定义的存储其值,未定义的存储INT_MIN
                else if(m_source_target_file_vec[0][j] == m_attri_uuls.m_LimitL
                        && m_source_target_file_vec[i][j].size() != 0)
                    limitL = stod(m_source_target_file_vec[i][j]);
                // limitU 定义的存储其值,未定义的存储INT_MAX
                else if(m_source_target_file_vec[0][j] == m_attri_uuls.m_LimitU
                        && m_source_target_file_vec[i][j].size() != 0)
                    limitU = stod(m_source_target_file_vec[i][j]);
            }
            // 将attri -> uul插入
            m_attri_uuls.m_attri_uuls.insert(make_pair(attri,
                                            Unit_Ul(unit,limitL,limitU)));
        }

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::profile_get_attri_uuls";
        throw;
    }
}

void Targetfile_Valid_Data::profile_get_series_datas()
{
    /*
        数据的结构类型:
            获取每一组图像上的series所需要的信息,使用map<string,vector<vector<double>>>结构,
                string存储attri属性,二维矩阵中横坐标代表site的编号,纵坐标代表site的第几次repeat

        默认参数:
            设置的默认参数，与profile_get_uuls函数中的默认参数同理

        存储内容:
            对于空的主数据,以NULL_Number进行存储
    */
    try {
        int sites = m_site_parts.get_Max_Site_Number(); // 芯片的个数
        int parts = m_site_parts.get_Max_Part_Id(); // repeat的组数
        int site_dex = 0; // SITE_NUM在target_file表中的下标位置
        int part_dex = 1; // PART_ID在target_file表中的下标位置
        size_t begin_row_dex = get_source_vec_row_index_by_attri(div_attri) + 1; // 与profile_get_uuls函数中同理
        size_t begin_col_dex = get_source_vec_col_index_valid(); // 获取有效列

        for(size_t i = begin_row_dex;i < m_source_target_file_vec.size();i++)
        {
            string attri = m_source_target_file_vec[i][0]; // 用来记录该行的属性值
            vector<vector<double>> data(sites,vector<double>(parts,NULL_Number)); // 横坐标为芯片号，纵坐标为该芯片的第几次part，其中空数据以NULL_Number存储

            for(size_t j = begin_col_dex; j < m_source_target_file_vec[i].size();j++)
            {
                // 正式读取主数据群数据，填充vector<vector<double>> data
                if(m_source_target_file_vec[i][j] == "NULL")  continue;

                // 数据表中的site和part从1开始，程序中从0开始计数
                // 转化行列下标
                int row = stoi(m_source_target_file_vec[site_dex][j]) - 1;
                int col = stoi(m_source_target_file_vec[part_dex][j]) - 1;
                // 转换行列后，获取数据
                data[row][col] = stod(m_source_target_file_vec[i][j]);
            }
            // 添加属性的主数据进map
            m_series_datas.insert(make_pair(attri,data));// 乱序存储 但可以通过labels vector进行查看检索
        }

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::profile_get_series_datas";
        throw;
    }
}


void Targetfile_Valid_Data::profile_get_datas()
{
    /*
        将target_file中已经读取进程序的数据进行分析拆分解析获取所需数据
    */
    try {
        profile_get_site_parts();
        profile_get_attri_uuls();
        profile_get_series_datas();

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::profile_get_datas";
        throw;
    }
}


void Targetfile_Valid_Data::load_target_vec(const ifstream& ifs)
{
    /*
        读取target_file中的表格数据读入程序中,存储在m_source_target_file_vec中
                            他是一个vector<vector<string>>的结构
    */
    try {
        string s;
        vector<string> arrays;
        string array;
        ifstream& t_ifs = const_cast<ifstream&>(ifs);
        while(getline(t_ifs,s))
        {
            istringstream sin(s);

            while(getline(sin,array,','))
            {
                arrays.push_back(array);
            }
            m_source_target_file_vec.push_back(arrays);
            arrays.clear();
        }

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::load_target_vec";
        throw;
    }
}

bool Targetfile_Valid_Data::is_Integer_Numeric(const string &str)
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
        qDebug() << "Targetfile_Valid_Data::is_Integer_Numeric";
        throw;
    }
}

vector<vector<string>> Targetfile_Valid_Data::get_source_target_file_vec() noexcept
{
    return m_source_target_file_vec;
}

Site_Part Targetfile_Valid_Data::get_site_parts() noexcept
{
    return m_site_parts;
}

Attri_Unit_Ul Targetfile_Valid_Data::get_attri_uuls() noexcept
{
    return m_attri_uuls;
}

map<string,vector<vector<double>>> Targetfile_Valid_Data::get_series_datas() noexcept
{
    return m_series_datas;
}

pair<double,double> Targetfile_Valid_Data::get_attri_XI(const string& attri)
{
    /*
        返回的数值的取值范围:
            pair: [first,second] = [INT_MIN,INT_MAX]
    */
    try {
        double ans_min = INT_MAX;
        double ans_max = INT_MIN;
        // 获取属性对应的处理数据【在原本已经处理好的数据上进行遍历】
        vector<vector<double>> vec = m_series_datas[attri];
        // 有数据时的正常情况
        for(size_t i = 0;i < vec.size();i++)
        {
            for(size_t j = 0;j < vec[i].size();j++)
            {
                ans_max = max(ans_max,vec[i][j]);
                // 过滤有效的数据进行min的计算【因为数据为NULL的数值标记为了INT_MIN】
                if(vec[i][j] == INT_MIN) continue;
                ans_min = min(ans_min,vec[i][j]);
            }
        }
        // 数据全为NULL时的特殊情况
        if(ans_min == INT_MAX && ans_max == INT_MIN)
        {
            ans_min = INT_MIN;
            ans_max = INT_MAX;
        }
        return make_pair(ans_min,ans_max);//[最小值,最大值]

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::get_attri_XI";
        throw;
    }
}

// 获取属性在m_source_target_file_vec中所对应的列索引值
size_t Targetfile_Valid_Data::get_source_vec_row_index_by_attri(const string& attri)
{
    /*
        根据attri，通过依次遍历的方式，直接获取属性在targe_file中的横坐标索引数值
    */
    try {
        size_t i;
        for(i = 0;i < m_source_target_file_vec.size();i++)
        {
            if(m_source_target_file_vec[i][0] == attri)break;
        }
        return i;

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::get_source_vec_row_index_by_attri";
        throw;
    }
}

size_t Targetfile_Valid_Data::get_source_vec_col_index_valid()
{
    /*
        根据SITE_NUM中的整数数据，来作为有效列索引的起点位置，
            因为SITE_NUM的有效数值一定是int，并且与其他属性的列数据一一对应，因此适合作为有效列起点的参考标志

        返回值为SITE_NUM所在第0行中第一个为int的数据的col索引
    */
    try {
        size_t row; // SITE_NUM所在行
        size_t col;
        for(row = 0,col = 0;col < m_source_target_file_vec[row].size();col++)
        {
            if(is_Integer_Numeric(m_source_target_file_vec[row][col]))
                break;
        }
        return col;

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::get_source_vec_col_index_valid";
        throw;
    }
}

void Targetfile_Valid_Data::profile_labels() noexcept
{
    /*
        功能：
            维护类内的labels，动态分析每个不同文件的属性项目数据，用于遍历所有有效的attri对应的datas
        说明：
            1. 只有在获取SITE_NUM,PART_ID的数据时，才需要SITE_NUM,PART_ID的属性
            2. 在分析attri->uul，attri->series_datas时，都不需要SITE_NUM,PART_ID属性
                【有效数据从div_attri后开始】
    */
    try {
        size_t begin_row_dex = get_source_vec_row_index_by_attri(div_attri) + 1;
        for(size_t i = begin_row_dex;i < m_source_target_file_vec.size();i++)
        {
            this->labels.push_back(m_source_target_file_vec[i][0]);
        }

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::profile_labels";
        throw ;
    }
}

pair<double,double> Targetfile_Valid_Data::get_ul_compare_attri_XI(const string& attri)
{
    /*
        返回值范围:
            [first,second] = [INT_MIN,INT_MAX]

        功能:
            根据传入的attri,对其数据和limitUL大小进行判断,并获取一个最值的pair返回
    */
    try {
        pair<double,double> attri_XI = get_attri_XI(attri); // [first,second] = [INT_MIN,INT_MAX]
        auto uul = m_attri_uuls.m_attri_uuls[attri]; // attri -> uul
        pair<double,double> ul_XI = make_pair(uul.m_LimitL,uul.m_LimitU); // [first,second] = [INT_MIN,INT_MAX]
        pair<double,double> ans;

        // 两个数值比较,每个数值有两种情况,这时的比较实际只有两种情况:
        //      1. a b 都为正常值
        //      2. a b 其中一个为特殊值INT_*,特殊值的话,那么直接min->max / max->min则必然获取其有效且正确的最值
        // 获取最小值
        if((ans.first = min(ul_XI.first,attri_XI.first)) == INT_MIN)
        {
            ans.first = max(ul_XI.first,attri_XI.first);
        }
        // 获取最大值
        if((ans.second = max(ul_XI.second,attri_XI.second)) == INT_MAX)
        {
            ans.second = min(ul_XI.second,attri_XI.second);
        }

        return ans;

    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::get_ul_compare_attri_XI";
        throw;
    }
}


const vector<string> Targetfile_Valid_Data::get_labels() noexcept
{
    return labels;
}

bool Targetfile_Valid_Data::total_task(const ifstream &ifs)
{
    /*执行该类的总任务*/
    try {
        // 加载target_file
        load_target_vec(ifs);
        // 拆解加载的数据信息
        profile_get_datas();
        // 动态获取有效的属性项目信息
        profile_labels();
        return true;
    } catch (...) {
        qDebug() << "Targetfile_Valid_Data::total_task";
        throw;
    }
}























