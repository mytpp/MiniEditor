#include "textrow.h"
#include "textstructure.h"
#include "Visitor/abstractvisitor.h"


TextStructure::TextStructure()
{
    text.push_back(TextRow());
}
