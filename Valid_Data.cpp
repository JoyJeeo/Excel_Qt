#include "Valid_Data.h"

void Valid_Data::profile_get_site_parts()
{
    /*
        数据的起点位置：
            获取SITE_NUM 和 PART_ID
            默认为列表的第一行 和 第二行的数据
        存储：
            存储数据在m_site_parts.m_site_part当中，其为一个map<string,vector<int>>结构
    */
    for(size_t i = 0;i < 2;i++)
    {
        string attri;
        for(size_t j = 0;j < m_source_target_file_vec[i].size();j++)
        {
            if(j == 0) attri = m_source_target_file_vec[i][j];
            if(is_Integer_Numeric(m_source_target_file_vec[i][j]))
            {
                m_site_parts.m_site_part[attri].push_back(stoi(m_source_target_file_vec[i][j]));
            }
        }
    }
}

void Valid_Data::profile_get_attri_uuls(const string& normal_attri = "PART_ID")
{
    /*
        数据的起点位置：
            1. 使用默认参数，normal_attri = "PART_ID"，
                定义uul获取数据的起始attri的前一位属性
                【因为前一位PART_ID可能是一个固定不变的末尾属性】【这样方便作为两组数据的的拆分】
            2. 默认情况下以PART_ID为两组数据拆分的分界点，使用默认参数留住接口位置，为后续可能的修改做准备

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
    */

// 获取起点获取数据的位置
size_t begin_row_dex = get_source_vec_row_index_by_attri(normal_attri) + 1;
size_t end_col_dex = get_source_vec_col_index_valid();

    for(size_t i = begin_row_dex;i < m_source_target_file_vec.size();i++)
    {
        string attri=m_source_target_file_vec[i][0]; // 存储属性名称
        string unit="";
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
                                        Attri_UL_Unit_NUMBER::UL_Unit_NUMBER(unit,limitL,limitU)));
    }

    // 扫描target_file表中的第一行数据
//    for(size_t i = 0,j = 0;j < m_source_target_file_vec[i].size();j++)
//    {
//        string attri;
//        // 跳过非Unit,LimitL,LimitU的第一行数据
//        if(m_source_target_file_vec[i][j] != "Unit" ||
//                m_source_target_file_vec[i][j] != "LimitL" ||
//                m_source_target_file_vec[i][j] != "LimitU")
//            continue;

//        // 获取属性 Unit,LimitL,LimitU 之一的名称
//        attri = m_source_target_file_vec[i][j];
//        // 使用列索引j，从开始获取数据的表项索引处开始存储数据
//        for(size_t k = begin_dex;k < m_source_target_file_vec.size();k++)
//        {
//            if(attri == "Unit")
//                // 存储size = 0的字符串进去【unit为空，画图时，不显示单位即可】
//                m_uuls.m_units.push_back(m_source_target_file_vec[k][j]);
//            else {
//                // UL如果为空【LimitL就存储INT_MIN,LimitU就存储INT_MAX】
//                if(m_source_target_file_vec[k][j].size() == 0){
//                    if(attri == m_uuls.LimitL)
//                    {
//                        m_uuls.m_uls[attri].push_back(INT_MIN);
//                    }
//                    if(attri == m_uuls.LimitU)
//                    {
//                        m_uuls.m_uls[attri].push_back(INT_MAX);
//                    }

//                }
//                else m_uuls.m_uls[attri].push_back(stod(m_source_target_file_vec[k][j]));
//            }
//        }
//    }
}

void Valid_Data::profile_get_series_datas(const string& normal_attri = "PART_ID")
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
int sites = m_site_parts.get_Max_Site_Number(); // 芯片的个数
int parts = m_site_parts.get_Max_Part_Id(); // repeat的组数
int site_dex = 0; // SITE_NUM在target_file表中的下标位置
int part_dex = 1; // PART_ID在target_file表中的下标位置
size_t begin_row_dex = get_source_vec_row_index_by_attri(normal_attri) + 1; // 与profile_get_uuls函数中同理
size_t begin_col_dex = get_source_vec_col_index_valid(); // 获取有效列

    for(size_t i = begin_row_dex;i < m_source_target_file_vec.size();i++)
    {
string attri = m_source_target_file_vec[i][0]; // 用来记录该行的属性值
vector<vector<double>> data(sites,vector<double>(parts,NULL_Number)); // 横坐标为芯片号，纵坐标为该芯片的第几次part，其中空数据以NULL_Number存储
//bool flage = true; // 观念值，用来手动设置哪些数据是不需要处理的数据【就像电路板上的0 1数据代表的含义一样，给数据赋予自己所谓的意义，执行对应操作】
        for(size_t j = begin_col_dex; j < m_source_target_file_vec[i].size();j++)
        {
            // 跨越无效数据区 // 直接进入数据区，不再需要跨越数据区获取数据
//            if(j == 0) attri = m_source_target_file_vec[i][j];
//            if(m_source_target_file_vec[i][j].size() != 0 && flage) continue;
//            if(m_source_target_file_vec[i][j].size() == 0) {
//                flage = false;
//                continue;
//            }
            // 正式读取主数据群数据，填充vector<vector<double>> data
            if(m_source_target_file_vec[i][j] == "NULL")  continue;
            // 转换数据后，获取数据
            data[stoi(m_source_target_file_vec[site_dex][j])][stoi(m_source_target_file_vec[part_dex][j])] = stod(m_source_target_file_vec[i][j]);
        }
        // 添加属性的主数据进map
        m_series_datas.insert(make_pair(attri,data));// 乱序存储 但可以通过labels vector进行查看检索
    }
}


void Valid_Data::profile_get_datas()
{
    /*
        将target_file中已经读取进程序的数据进行分析拆分解析获取所需数据
    */
    profile_get_site_parts();
    profile_get_attri_uuls();
    profile_get_series_datas();
}


void Valid_Data::load_target_vec(const ifstream& ifs)
{
    /*
        读取target_file中的表格数据读入程序中,存储在m_source_target_file_vec中
                            他是一个vector<vector<string>>的结构
    */
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
}

bool Valid_Data::is_Integer_Numeric(const string &str)
{
    /*
        通过判断str中是否有非0-9的数据，来判断str是否为一个存储数值的字符串

        该函数只能判断，str是否为一个非小数的数值数据
    */
    auto it = str.begin();
    while (it != str.end() && std::isdigit(*it)) {
        it++;
    }
    return !str.empty() && it == str.end();
}

vector<vector<string>> Valid_Data::get_source_target_file_vec()
{
    return m_source_target_file_vec;
}

SITE_PART Valid_Data::get_site_parts()
{
    return m_site_parts;
}

Attri_UL_Unit_NUMBER Valid_Data::get_attri_uuls()
{
    return m_attri_uuls;
}

map<string,vector<vector<double>>> Valid_Data::get_series_datas()
{
    return m_series_datas;
}

pair<double,double> Valid_Data::get_attri_XI(const string& attri)
{
    /*
        返回的数值的取值范围:
            pair: [first,second] = [INT_MIN,INT_MAX]
    */
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
}

// 获取属性在m_source_target_file_vec中所对应的列索引值
size_t Valid_Data::get_source_vec_row_index_by_attri(const string& attri)
{
    /*
        根据attri，通过依次遍历的方式，直接获取属性在targe_file中的横坐标索引数值
    */
    size_t i;
    for(i = 0;i < m_source_target_file_vec.size();i++)
    {
        if(m_source_target_file_vec[i][0] == attri)break;
    }
    return i;
}

size_t Valid_Data::get_source_vec_col_index_valid()
{
    /*
        根据SITE_NUM中的整数数据，来作为有效列索引的起点位置，
            因为SITE_NUM的有效数值一定是int，并且与其他属性的列数据一一对应，因此适合作为有效列起点的参考标志

        返回值为SITE_NUM所在第0行中第一个为int的数据的col索引
    */
    size_t row; // SITE_NUM所在行
    size_t col;
    for(row = 0,col = 0;col < m_source_target_file_vec[row].size();col++)
    {
        if(is_Integer_Numeric(m_source_target_file_vec[row][col]))
            break;
    }
    return col;
}

pair<double,double> Valid_Data::get_ul_compare_attri_XI(const string& attri)
{
    /*
        返回值范围:
            [first,second] = [INT_MIN,INT_MAX]

        功能:
            根据传入的attri,对其数据和limitUL大小进行判断,并获取一个最值的pair返回
    */
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
}


const vector<string> Valid_Data::get_labels()
{
    return labels;
}























