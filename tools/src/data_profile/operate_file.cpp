#include "../../include/data_profile/operate_file.h"

// 目标表的长和宽
const int cols_num = 45;
const int rows_num = 45;


const string task01()
{
    /*将文件处理后，将目标文件名进行返回*/
    ifstream ifs = input_file_open(IN_FILE_NAME);
    vector<vector<string>> datas = tackle_file(ifs);
    ifs.close();
    ofstream ofs = output_file_open(OUT_FILE_NAME);
    save_tackle_datas(ofs,datas);
    ofs.close();
    return OUT_FILE_NAME;
}

ifstream input_file_open(const string& open_File_Name)
{
    /*打开输入文件*/
    ifstream ifs;
    ifs.open(open_File_Name);
    if(!ifs.is_open())
    {
        cerr<<"file open fail"<<endl;
        exit(1);
    }
    return ifs;
}
ofstream output_file_open(const string& open_File_Name)
{
    /*打开输出文件*/
    ofstream ofs;
    ofs.open(open_File_Name,ios::out | ios::trunc);

    return ofs;
}
vector<vector<string>> tackle_file(const ifstream& ifs)
{
    /*从输入文件的内容中，获取和筛选需要的数据，暂时存入容器中进行返回
        可以使用test_datas将返回的矩阵数据进行打印测试
    */
    vector<vector<string>> ans;
    vector<vector<string>> all_array = tackle_file_all(ifs);
    vector<vector<string>> target_arrays = tackle_file_get_target(all_array);
    vector<vector<string>> rows_array = tackle_file_rows(target_arrays);
    vector<vector<string>> cols_array = tackle_file_cols(target_arrays);
    ans = tackle_file_get_ans(rows_array,cols_array);

    return ans;
}

vector<vector<string>> tackle_file_all(const ifstream& ifs)
{
    /*将整个xlsx表格读入程序中*/
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
    return all_array;
}
vector<vector<string>> tackle_file_get_target(const vector<vector<string>>& all_array)
{
    /*获取目标表格的数据载入程序中*/
    vector<vector<string>> target_arrays;
    vector<vector<string>>& t_all_array = const_cast<vector<vector<string>>&>(all_array);

    // cout<<all_array.size()<<" "<<all_array[0].size()<<endl;

    for(size_t i = 0;i < t_all_array.size();i++)
    {
        if(t_all_array[i].size() != cols_num)continue;
        else {
            for(size_t j = i;j < t_all_array.size();j++)
            {
                while(t_all_array[j].size() != cols_num) t_all_array[j].push_back(""); // 补0
                target_arrays.push_back(t_all_array[j]);
            }
            break;
        }
    }

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
}
vector<vector<string>> tackle_file_rows(const vector<vector<string>>& target_arrays)
{
    /*将目标数据以行形式存储*/
    vector<vector<string>> rows_array = target_arrays;
    // cout<<target_arrays[4][44].size()<<endl;

    // test_datas(target_arrays);

    return rows_array;
}
vector<vector<string>> tackle_file_cols(const vector<vector<string>>& target_arrays)
{
    /*将目标数据以列形式存储*/
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

vector<vector<string>> tackle_file_get_ans(const vector<vector<string>>& rows_array,const vector<vector<string>>& cols_array)
{
    /*主要使用列数据的处理结果，将表项的属性数据进行按要求的进一步处理后输出*/
    vector<vector<string>> ans;
    // map<string,vector<string>> m = tackle_file_get_maps(cols_array);
    for(auto x : cols_array){
        if(find(labels.begin(),labels.end(),x[0]) != labels.end())
        {
            ans.push_back(x);
        }
    }

    // test_datas(cols_array);

    for(size_t i = 0;i < cols_array.size(); i++)
    {
        if(cols_array[i][0] == "PASSFG")
        {
            for(size_t j = 0;j < cols_array[i].size();j++)
            {
                if(cols_array[i][j] == "FALSE") // i已经是一个定值了
                {
                    for(size_t k = 2;k < ans.size();k++) // 注意：是通过未处理的cols与处理过的ans矩阵进行比较得到的结果
                    {
                        if(ans[k][j].size() == 0) ans[k][j] = "NULL";
                    }
                }
            }
            break;
        }  
    }

    // test_datas(ans);

    return ans;
}

void save_tackle_datas(const ofstream& ofs,const vector<vector<string>>& datas)
{
    /*获得输出文件，将处理筛选过的数据写入输入文件中，作为中间文件进行使用*/
    ofstream& t_ofs = const_cast<ofstream&>(ofs);
    for(size_t i = 0;i < datas.size(); i++)
    {
        for(size_t j = 0;j < datas[i].size();j++)
        {
            t_ofs<<datas[i][j]<<",";
        }
        t_ofs<<endl;
    }
}

void draw_datas(const string& draw_File_Name)
{
    /*使用中间文件，将中间文件的数据打开后，对其中内容进行绘制*/
    ifstream ifs = input_file_open(draw_File_Name);
    cout<<"drawing..."<<endl;
    ifs.close();
}

// test
void test_datas(const vector<vector<string>>& arrays)
{
    // 展示处理的结果
    for(size_t i = 0;i < arrays.size(); i++)
    {
        for(size_t j = 0;j < arrays[i].size();j++)
        {
            cout<<arrays[i][j]<<" ";
        }
        cout<<endl;
    }
}
