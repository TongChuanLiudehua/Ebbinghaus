#include <map>
#include <queue>
#include <iostream>
#include <cstring>
#include "../head/Job.h"
#include "../head/cmdt.h"

using namespace std;

int __G__stat = 0;
int a_idx = 0;
int c_idx = 0;
char cmd[100];
char arg[100];

// jump table
map<char, int> s1 = {
        {'-', 2},
        {'s', 11},
        {'a', 24},
        {'A', 27},
        {'d', 31},
        {'f', 44},
        {'F', 46},
        {'r', 50},
};


map<char, int> s2 = {
        {'p', 3},
        {'s', 11},
        {'a', 21},
        {'d', 31},
        {'h', 61},
        {'f', 41},
        {'r', 71},
};

map<char, int> s3 = {
        {'a', 4},
        {'c', 5},
        {'d', 6},
};

map<char, int> q3 = {
        {'+', 24},
        {'s', 27},
};

map<char, int> n2 = {
        {'o', 43},
        {'e', 49},
};

map<char, int> n3 = {
        {'+', 44},
        {'s', 46},
};

void help_msg()
{
    cout <<
    "Usage of commands:\n" \
    "Management Of Project:\n" \
    "   --pa [name]     create a project that named after name\n" \
    "   --pc [name]     change the default project to name\n"\
    "   --pd [name]     delete project named after name\n"
    "Management Of Job:\n" \
    "   --start     start up the default project\n" \
    "               if the project have already started, this command will\n"\
    "               upgrade the job tables\n" \
    "   --done      finish the project\n"
    "   --add+ [job]\n"\
    "       -a [job]        add a single job into project\n"
    "   --adds [job,...]\n" \
    "       -A [job,...]    add a list of jobs into project\n"
    "                       note:you can use \"\" to brace job list\n"
    "   --fro  [job]\n"\
    "      -f  [job]        stop a job, and making the job working around\n"
    "   --fros [job,...]\n"\
    "       -F [job,...]    stop a list of job, and making them working\n"
    "   --free [job]        free a stopped job\n" \
    "   --remove [job]      remove a job from project"
    << endl;
}
void nextJump(int stat, char _nex)
{
    map<char, int>::iterator look;
    if (stat == 1)
        look = s1.find(_nex);
    else if (stat == 3)
        look = s3.find(_nex);
    else if (stat == 2)
        look = s2.find(_nex);
    else if (stat == 23)
        look = q3.find(_nex);
    else if (stat == 42)
        look = n2.find(_nex);
    else if (stat == 43)
        look = n3.find(_nex);
    else if (stat == 0)
    {
        if (_nex == '-')
        {
            __G__stat = 1;
            return;
        } else
        {
            cout << "Entry error" << endl;
            exit(0);
        }
    } else
        return;

    // miss table
    if (stat == 1 && look == s1.end())
    {
        __G__stat = -1;
        throw_parse_error(__G__stat);
    } else if (stat == 2 && look == s2.end())
    {
        __G__stat = -2;
        throw_parse_error(__G__stat);
    } else if (stat == 3 && look == s3.end())
    {
        __G__stat = -3;
        throw_parse_error(__G__stat);
    } else if (stat == 23 && look == q3.end())
    {
        __G__stat = -23;
        throw_parse_error(__G__stat);
    } else if (stat == 42 && look == n2.end())
    {
        __G__stat = -42;
        throw_parse_error(__G__stat);
    } else if (stat == 43 && look == n3.end())
    {
        __G__stat = -43;
        throw_parse_error(__G__stat);
    } else
    {
        __G__stat = look->second;
    }
}

inline void set_error_code_print(int code, const char *info, const char *withcmd = nullptr)
{
    cout << info;
    if (withcmd)
        cout << withcmd << endl;
}

void throw_parse_error(int stat)
{
    if (stat == -15)
        set_error_code_print(-15, "'start' expected not :", cmd);
    if (stat == -34)
        set_error_code_print(-34, "'done' expected not :", cmd);
    if (stat == -1)
        set_error_code_print(-1, "'-' expected, not :", cmd);
    if (stat == -2)
        set_error_code_print(-2, "--pa|d|c|--start|--f..|--a.. expected not :", cmd);
    if (stat == -3)
        set_error_code_print(-3, "--pa|pc|pd expected not :", cmd);
    if (stat == -4)
        set_error_code_print(-4, "--pd need arg");
    if (stat == -5)
        set_error_code_print(-5, "--pc need arg");
    if (stat == -6)
        set_error_code_print(-6, "--pd need arg");
    if (stat == -23)
        set_error_code_print(-23, "--add expected not :", cmd);
    if (stat == -24)
        set_error_code_print(-24, "--add+ need arg");
    if (stat == -27)
        set_error_code_print(-27, "--adds need arg");
    if (stat == -42)
        set_error_code_print(-42, "--free|fro|fros expected, not :", cmd);
    if (stat == -43)
        set_error_code_print(-42, "--fro|fros expected ,not :", cmd);
    if (stat == -49)
        set_error_code_print(-49, "--free need arg");
    if (stat == -50)
        set_error_code_print(-50, "--free expected ,not :", cmd);
    if (stat == -44)
        set_error_code_print(-44, "--frozen need arg");
    if (stat == -46)
        set_error_code_print(-46, "--frozen need arg");
    if (stat == -64)
        set_error_code_print(-64, "--help expected, not", cmd);
    if (stat == -71)
        set_error_code_print(-71, "--remove need arg");
    if(stat== -76)
        set_error_code_print(-76, "--remove expected, not", cmd);
}

void dispatch()
{
    switch (__G__stat)
    {
        case 11:
            pstart();
            break;
        case 31:
            pdone();
            break;
        case 4:
            pa();
            break;
        case 5:
            pc();
            break;
        case 6:
            pd();
            break;
        case 41:
            f_head();
            break;
        case 43:
            fro_head();
            break;
        case 44:
            fro();
            break;
        case 46:
            fros();
            break;
        case 49:
            jfree();
            break;
        case 21:
            add_head();
            break;
        case 24:
            add();
            break;
        case 27:
            adds();
            break;
        case 61:
            help();
            break;
        case 71:
            remove();
            break;
    }
}

inline char getChar()
{
    return cmd[c_idx++];
}

inline void ungetChar()
{
    c_idx--;
}


Job<> jp("F:\\cppProject\\AAebb\\project");
Project pjk("F:\\cppProject\\AAebb\\dir", "F:\\cppProject\\AAebb\\project");

void call(int stat)
{

    switch (stat)
    {
        case 64:
            help_msg();
            break;
        case 15:
            jp.startup();
            jp.join();
            break;
        case 34:
            jp.finishJob();
            jp.put();
            break;
        case 4:
            pjk.createProj(arg);
            pjk.put();
            break;
        case 5:
            pjk.changeProj(arg);
            jp.pre_load();
            break;
        case 6:
            pjk.deleteProj(arg);
            pjk.put();
            break;
        case 24:
            jp.addJob(job_info_table_item(0, string(arg), 0));
            jp.put();
            break;
        case 27:
            jp.addJobs(string(arg));
            jp.put();
            break;
        case 50:
            jp.unfreezeJob(string(arg));
            jp.put();
            break;
        case 44:
            jp.freezeJob(string(arg));
            jp.put();
            break;
        case 46:
            cout << "call job.frozens() with arg=" << arg << endl;
            break;
        case 76:
            jp.deleteJobByName(string(arg));
            jp.put();
            break;
    }
    arg[0] = 0;
    a_idx = 0;
}

void run()
{
    if (__G__stat < 0)
    {
        cout << "roll back" << endl;
        return;
    }

    while (!exec_queue.empty())
    {
        pair<int, string> x = exec_queue.front();
        exec_queue.pop();
        strcpy(arg, x.second.c_str());
        call(x.first);
    }
}

void head()
{
    for (char x = getChar();; x = getChar())
    {
        if (__G__stat == 0 && x == '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 1 && x == '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 1 && isalpha(x))
            nextJump(__G__stat, x);
        else if (__G__stat == 2 && x != '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 3 && x != '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 23 && x != '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 42 && x != '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 43 && x != '-')
            nextJump(__G__stat, x);
        else
        {
            ungetChar();
            break;
        }
    }
};
inline void entry_check(int stat)
{
    if (__G__stat != stat)
    {
        cout << "Entry error" << endl;
        exit(0);
    }
}

inline void template_set_G_stat(int start_stat, int end_stat, const char *stat_nex, int idx_nex = 0)
{
    if (start_stat != end_stat)
    {
        char x = getChar();
        if (*(stat_nex + idx_nex) == x)
        {
            __G__stat = ++start_stat;
            template_set_G_stat(start_stat, end_stat, stat_nex, ++idx_nex);
        } else
        {
            return;
        }
    }
}

inline void without_arg_check_exec(int error_code)
{
    if (__G__stat != error_code)
    {
        __G__stat = -error_code;
        throw_parse_error(__G__stat);
    } else
    {
        pair<int, string> exe(__G__stat, "");
        exec_queue.push(exe);
        __G__stat = 0;
    }
}

inline void with_arg_check_exec(int error_code)
{
    if (a_idx == 0)
    {
        __G__stat = error_code;
        throw_parse_error(__G__stat);
    } else
    {
        arg[a_idx] = 0;
        exec_queue.push(make_pair(__G__stat, string(arg)));
        __G__stat = 0;
    }
}

inline int eat_arg(int error_code)
{
    a_idx = 0; //eat char from the beginning
    for (char x = getChar(); x != 0 && x != '-'; x = getChar())
    {
        x != '"' ? arg[a_idx++] = x : x;
    }
    ungetChar();
    with_arg_check_exec(error_code);
    return a_idx;
}
void help()
{
    entry_check(61);
    template_set_G_stat(61,64,"elp");
    without_arg_check_exec(64);
}
void pstart()
{
    entry_check(11);
    template_set_G_stat(11, 15, "tart", 0);
    without_arg_check_exec(15);
}

void pdone()
{
    entry_check(31);
    template_set_G_stat(31, 34, "one");
    without_arg_check_exec(34);
}

void add_head()
{
    entry_check(21);
    template_set_G_stat(21, 23, "dd", 0);
    if (__G__stat != 23)
    {
        __G__stat = -23;
        throw_parse_error(__G__stat);
    }
}

void f_head()
{
    entry_check(41);
    template_set_G_stat(41, 42, "r", 0);
    if (__G__stat != 42)
    {
        __G__stat = -42;
        throw_parse_error(__G__stat);
    }

}

void adds()
{
    entry_check(27);
    eat_arg(-27);
}

void add()
{
    entry_check(24);
    eat_arg(-24);
}

void remove()
{
    entry_check(71);
    template_set_G_stat(71, 76,"emove");
    if (__G__stat != 76)
    {
        __G__stat = -76;
        throw_parse_error(__G__stat);
    }
    eat_arg(-71);
}

void jfree()
{
    entry_check(49);
    template_set_G_stat(49, 50, "e", 0);
    if (__G__stat != 50)
    {
        __G__stat = -50;
        throw_parse_error(__G__stat);
    }
    eat_arg(-49);
}

void fro()
{
    entry_check(44);
    eat_arg(-44);
}

void fros()
{
    entry_check(46);
    eat_arg(-46);
}

void fro_head()
{
    entry_check(42);
    template_set_G_stat(42, 43, "o", 0);
}

inline void template_p_(int stat)
{
    entry_check(stat);
    eat_arg(-stat);
}

void pc()
{
    template_p_(5);
}

void pa()
{
    template_p_(4);
}

void pd()
{
    template_p_(6);
}



