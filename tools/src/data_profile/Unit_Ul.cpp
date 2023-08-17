#include "tools/include/data_profile/Unit_Ul.h"



bool Unit_Ul::operator!=(const Unit_Ul &uul)
{
    if(uul.m_Unit != this->m_Unit ||
            uul.m_LimitL != this->m_LimitL ||
            uul.m_LimitU != this->m_LimitU)
        return true;

    return false;
}

bool Unit_Ul::operator==(const Unit_Ul &uul)
{
    return !operator!=(uul);
}

Unit_Ul::operator string()
{
    string str = "Unit: " + m_Unit +
            " LimitL: " + to_string(m_LimitL) +
            " LimitU: " + to_string(m_LimitU);

    return str;
}
