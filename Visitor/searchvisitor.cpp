#include "searchvisitor.h"
#include <QDebug>

/*构造函数*/
/*初始化*/
/*构造format模式串的next数组*/
SearchVisitor::SearchVisitor(QString format, Qt::CaseSensitivity cs)
    :target(format), sensitivity(cs), next(new int [format.size()]),
      index(0), row_count(0), column_count(0)
{
    int i = 0;
    next[0] = -1;  //next[0]初始化为-1.  -1表示不存在相同的最大前缀和最大后缀
    int j = -1;    //j初始化为-1
    while( i < format.size()-1 )
    {
        if(j==-1 || equal(format[i],format[j] ) )
        {
            i++;
            j++;
            next[i]=j;
        }
        else
            j = next[j];
    }
}


/*按照cs标准，判断a与b是否相等*/
bool SearchVisitor:: equal(QChar a, QChar b)
{
    if(sensitivity==Qt::CaseSensitive)
       return a == b;
    else
        return a.toLower()==b.toLower();
}

/*析构函数*/
SearchVisitor::~SearchVisitor()
{
    delete [] next;
}


/*遍历kmp算法求匹配*/
bool SearchVisitor::visit(QChar& element)
{   
    std::pair<int,int> position(0,0);
    if(element!='\n' && index < target.size())
    {
       while(  index != -1 && !equal(element,target[index])   )
           index=next[index];
       index++;
       if (index == target.size())
       {
           position.second = column_count-target.size()+1;
           position.first = row_count;
           result.push_back(position);
           index = 0;
       }
       column_count++;
       return true;
    }

    else if(element=='\n')
    {
        row_count++;
        column_count =0;
        index=0;
        return true;
    }

    return false;
}

/*返回模式串format的内容*/
const QString& SearchVisitor::getFormat() const
{
    return target;
}


/*返回匹配结果*/
std::vector<std::pair<int,int>>& SearchVisitor::getResult()
{

    return result;
}

/*标识匹配是否成功*/
bool SearchVisitor::noResult()
{
    return result.empty();
}


