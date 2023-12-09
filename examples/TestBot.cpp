#include "System/Output.h"
#include <gum-system.h>
#include <gum-discord.h>


int main()
{
    Bot* pBot = new Bot("Mzg2NTUzNTY5NDU4NzgyMjA5.GCJLsi.d12eWuF_-c-PTn7W8rXAAm3GTct566A518sXqE");

    Gum::Output::print("Username:    " + pBot->getUsername().toString());
    Gum::Output::print("Description: " + pBot->getDescription().toString());
    Gum::Output::print("ID:          " + pBot->getID().toString());

    Gum::_delete(pBot);
    return 0;
}