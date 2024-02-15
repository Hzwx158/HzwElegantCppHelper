#include "./hstd/hCppHelper.h"
#include <vector>
UseStdIO
using namespace hstd;
using std::string;
using std::vector;
class StuInfo:hstd::TComparable<StuInfo>{
    size_t id;
    string name;
    int score;
public:
    StuInfo(string name_="", size_t id_=0, int score_=0):name(name_),id(id_),score(score){}
    //由于继承了TComparable，所以要重载compare
    int compare(const StuInfo &obj) const override
    {
        if(this==&obj) return 0;
        if(score==obj.score){
            long long r=id-obj.id;
            return r?r/abs(r):0;
        }
        int r=score-obj.score;
        return r?r/abs(r):0;
    }
    //getter和setter
    GET(name) const {return "["+name+"]";}
    GET(id) const {return id;}
    GET(score) const {return score;}
    SET(score, int s){score=s;}
    //输入输出宏
    H_IOABLE(StuInfo)
};
std::istream &operator>>(std::istream &ism, StuInfo &obj)
{
    cout<<"请按顺序输入姓名、学号、分数"<<endl;
    ism>>obj.name>>obj.id>>obj.score;
    return ism;
}
std::ostream &operator<<(std::ostream &osm, const StuInfo &obj)
{
    osm<<"StuInfo{\n\t.name="<<obj.name
    <<"\n\t.id="<<obj.id
    <<"\n\t.score="<<obj.score
    <<"\n}";
    return osm;
}
void showArr(const vector<StuInfo> &arr){
    for(auto &item:arr) cout<<item<<endl;
}
int main()
{
    vector<StuInfo> arr;
    int cnt=0;
    //输入
    cout<<"输入学生个数:";
    cin>>cnt;
    StuInfo input;
    for(int i=0;i<cnt;++i){
        cin>>input;
        arr.push_back(input);
    }
    //筛选
    constexpr int scoreMax=1000;
    auto scoreIn=PRED([scoreMax](const vector<StuInfo> &arr, int b){
        vector<StuInfo> res;
        for(auto &item:arr)
            if(item.get_score()<=scoreMax&&item.get_score()>=b) 
                res.push_back(item);
        return res;
    });//把lambda变“谓词”，可以像C++20的管道运输符使用
    auto show=PRED(showArr);//可以直接把定义好的函数转化
    arr|scoreIn(10)|show;//统计10分以上的并输出
    //输出第一个学生的姓名
    auto getName=memFunc(StuInfo::get_name);//这是把成员函数变成普通“函数”
    cout<<getName(arr[0])<<endl;//等价于arr[0].get_name()
    return 0;
}