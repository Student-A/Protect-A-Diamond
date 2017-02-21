//==========Eval.cpp===========

#include <string>
#include <stdlib.h>
#include <sstream>
#include "Eval.h"
#include <iostream>
#include <math.h>
#include "Report.h"


std::string GetArg::GetArg0(std::string line, std::string tag, std::string exception)
{
    std::string exceptions=exception.c_str()=="ALPHS"?"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ":exception;
    bool doexception=exceptions.c_str()==""?false:true;
    bool isechar=false;
    int tagpos= line.find(tag)+tag.length();
    temp1="";
    for (int a=tagpos; a<line.length(); a++)
    {
        if (doexception)
        {
            for (int b=0; b<exceptions.length(); b++)
            {
                if (line.substr(a, 1)==exceptions.substr(b, 1))
                {
                    isechar=true;
                    break;
                }
            }
        }
        if (isdigit(line[a])||doexception)
        {
            for(;;)
            {
                if((!isdigit(line[a]) && !doexception)||(doexception && !isechar && !isdigit(line[a])))return temp1;
                temp1+=line.substr(a++, 1);
                if(a==line.length()) return temp1;
                if (!(line.substr(a,1).c_str()==" ")) return "FAILED!";
            }
        }
    }
    return "";
}

std::string GetArg::GetArg1(std::string line, std::string tag, std::string exception, int noofparas, std::string seperator)
{
    std::string exceptions=exception.c_str()=="ALPHS"?"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ":exception;
    bool doexception=exceptions.c_str()==""?false:true;
    bool isechar=false;
    int tagpos= line.find(tag)+tag.length();
    temp1="";
    for (int a=tagpos; a<line.length(); a++)
    {
        if (doexception)
        {
            for (int b=0; b<exceptions.length(); b++)
            {
                if (line.substr(a, 1)==exceptions.substr(b, 1))
                {
                    isechar=true;
                    break;
                }
            }
        }
        if (isdigit(line[a])||doexception)
        {
            for(;;)
            {
                if((!isdigit(line[a]) && !doexception)||(doexception && !isechar && !isdigit(line[a])))return temp1;
                temp1+=line.substr(a++, 1);
                if(a==line.length()) return temp1;
                if (!(line.substr(a,1).c_str()==" ")) return "FAILED!";
            }
        }
    }
    return "";
}




bool Eval::BEval(std::string oper)
{
    operation=oper;
    while(operation.rfind("(")!=std::string::npos)
    {
        temp1=operation.rfind("(");
        temp3=operation.find(")", temp1);
        temp2=operation.substr(temp1+1,temp3-temp1-1);



        temp2=ISEQUAL(operation);
        temp2=ISNOTEQUAL(operation);
        temp2=ISGREATER(operation);
        temp2=ISSMALLER(operation);
        temp2=ISGREATERE(operation);
        temp2=ISSMALLERE(operation);
        temp2=AND(operation);
        temp2=OR(operation);

        operation.replace(temp1, temp3-temp1+1, temp2);
    }
    operation=ISEQUAL(operation);
    operation=ISNOTEQUAL(operation);
    operation=ISGREATER(operation);
    operation=ISSMALLER(operation);
    operation=ISGREATERE(operation);
    operation=ISSMALLERE(operation);
    operation=AND(operation);
    operation=OR(operation);
}


std::string Eval::ISEQUAL(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("==")!=std::string::npos)
    {
        int pluspos=a.find("==");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")
        	return a;
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+2])||(a.substr(temposha+2,1)=="-" && temposha==pluspos)||a.substr(temposha+2,1)=="."))
        {
            rarg += a.substr(temposha+2, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        if (rarg=="")
        	return a;
        irarg=temposha;
        temposha=a.length();

        if (atoi(larg.c_str())==atoi(rarg.c_str()))
        {
            a.replace(ilarg,irarg-ilarg+2, "1");
        }
        else
        {
            a.replace(ilarg,irarg-ilarg+2, "0");
        }

        larg="";
        rarg="";
    }
    return a;
}


std::string Eval::ISNOTEQUAL(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("!=")!=std::string::npos)
    {
        int pluspos=a.find("!=");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")
            return a;
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+2])||(a.substr(temposha+2,1)=="-" && temposha==pluspos)||a.substr(temposha+2,1)=="."))
        {
            rarg += a.substr(temposha+2, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        if (rarg=="")
            return a;
        irarg=temposha;
        temposha=a.length();

        if (atoi(larg.c_str())!=atoi(rarg.c_str()))
        {
            a.replace(ilarg,irarg-ilarg+2, "1");
        }
        else
        {
            a.replace(ilarg,irarg-ilarg+2, "0");
        }

        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::ISGREATER(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find(">")!=std::string::npos)
    {
        int pluspos=a.find(">");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")
            return a;
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+1])||(a.substr(temposha+1,1)=="-" && temposha==pluspos)||a.substr(temposha+1,1)=="."))
        {
            rarg += a.substr(temposha+1, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        if (rarg=="") return a;
        irarg=temposha;
        temposha=a.length();

        if (atoi(larg.c_str())>atoi(rarg.c_str()))
        {
            a.replace(ilarg,irarg-ilarg+1, "1");
        }
        else
        {
            a.replace(ilarg,irarg-ilarg+1, "0");
        }

        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::ISGREATERE(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find(">=")!=std::string::npos)
    {
        int pluspos=a.find(">=");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")return a;
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+2])||(a.substr(temposha+2,1)=="-" && temposha==pluspos)||a.substr(temposha+2,1)=="."))
        {
            rarg += a.substr(temposha+2, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        if (rarg=="")return a;
        irarg=temposha;
        temposha=a.length();

        if (atoi(larg.c_str())>=atoi(rarg.c_str()))
        {
            a.replace(ilarg,irarg-ilarg+2, "1");
        }
        else
        {
            a.replace(ilarg,irarg-ilarg+2, "0");
        }

        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::ISSMALLER(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("<")!=std::string::npos)
    {
        int pluspos=a.find("<");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")return a;
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+1])||(a.substr(temposha+1,1)=="-" && temposha==pluspos)||a.substr(temposha+1,1)=="."))
        {
            rarg += a.substr(temposha+1, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        if (rarg=="")return a;
        irarg=temposha;
        temposha=a.length();

        if (atoi(larg.c_str())<atoi(rarg.c_str()))
        {
            a.replace(ilarg,irarg-ilarg+1, "1");
        }
        else
        {
            a.replace(ilarg,irarg-ilarg+1, "0");
        }

        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::ISSMALLERE(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("<=")!=std::string::npos)
    {
        int pluspos=a.find("<=");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")return a;
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+2])||(a.substr(temposha+2,1)=="-" && temposha==pluspos)||a.substr(temposha+2,1)=="."))
        {
            rarg += a.substr(temposha+2, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        if (rarg=="")return a;
        irarg=temposha;
        temposha=a.length();

        if (atoi(larg.c_str())<atoi(rarg.c_str()))
        {
            a.replace(ilarg,irarg-ilarg+2, "1");
        }
        else
        {
            a.replace(ilarg,irarg-ilarg+2, "0");
        }

        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::AND(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("&&")!=std::string::npos)
    {
        int pluspos=a.find("&&");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+2])||(a.substr(temposha+2,1)=="-" && temposha==pluspos)||a.substr(temposha+2,1)=="."))
        {
            rarg += a.substr(temposha+2, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        irarg=temposha;
        temposha=a.length();

        if (atoi(larg.c_str())==1 && atoi(rarg.c_str())==1)
        {
            a.replace(ilarg,irarg-ilarg+2, "1");
        }
        else
        {
            a.replace(ilarg,irarg-ilarg+2, "0");
        }

        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::OR(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("||")!=std::string::npos)
    {
        int pluspos=a.find("||");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+2])||(a.substr(temposha+2,1)=="-" && temposha==pluspos)||a.substr(temposha+2,1)=="."))
        {
            rarg += a.substr(temposha+2, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        irarg=temposha;
        temposha=a.length();

        if (atoi(larg.c_str())==1 || atoi(rarg.c_str())==1)
        {
            a.replace(ilarg,irarg-ilarg+2, "1");
        }
        else
        {
            a.replace(ilarg,irarg-ilarg+2, "0");
        }

        larg="";
        rarg="";
    }
    return a;
}


Eval::Eval(std::string oper)
{
    if (oper.find(".")!=std::string::npos)
        int www=1213;
    operation=oper;
    while(operation.rfind("(")!=std::string::npos)
    {
        temp1=operation.rfind("(");
        temp3=operation.find(")", temp1);
        temp2=operation.substr(temp1+1,temp3-temp1-1);
        std::string temp4=operation.substr(temp1+1,temp3-temp1-1);
        temp2=POWER(temp2);                //P
        temp2=MULTIPLYORDIVIDE(temp2);     //D/M
        temp2=ADDORSUBTRACT(temp2);        //A/s
        if (temp4!=temp2)
        {
            operation.replace(temp1, temp3-temp1+1, temp2);
        }
        else
        {
            operation.replace(temp1, 1, "[");
            operation.replace(temp3, 1, "]");
        }
    }
    while (operation.find("--")!=std::string::npos)
    {
        operation.replace(operation.find("--"), 2, "+");   
    }
    while (operation.find("-+")!=std::string::npos)
    {
    	operation.replace(operation.find("-+"), 2, "-");
    }
    while (operation.find("+-")!=std::string::npos)
    {
    	operation.replace(operation.find("+-"), 2, "-");
    }
    operation=POWER(operation);      //P
    operation=MULTIPLYORDIVIDE(operation);
    operation=ADDORSUBTRACT(operation);

    while (operation.find("[")!=std::string::npos) operation.replace(operation.find("["), 1, "(");
    while (operation.find("]")!=std::string::npos) operation.replace(operation.find("]"), 1, ")");
}






std::string Eval::DIVIDE(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("/")!=std::string::npos)
    {
        int pluspos=a.find("/");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")return a;
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+1])||(a.substr(temposha+1,1)=="-" && temposha==pluspos)||a.substr(temposha+1,1)=="."))
        {
            rarg += a.substr(temposha+1, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        if (rarg=="")return a;
        irarg=temposha;
        temposha=a.length();
        std::string quotient=stringify(atof(larg.c_str())/atof(rarg.c_str()));
        if (quotient.find("e")!=std::string::npos)
        {
            quotient="0";
        }
        if (larg.find("-")==0)
        {
            if (quotient.find("-")!=std::string::npos && quotient.find("-")>1) quotient="+"+quotient;
        }
        a.replace(ilarg,irarg-ilarg+1, quotient);
        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::MULTIPLY(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("*")!=std::string::npos)
    {
        int pluspos=a.find("*");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")return a;
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+1])||(a.substr(temposha+1,1)=="-" && temposha==pluspos)||a.substr(temposha+1,1)=="."))
        {
            rarg += a.substr(temposha+1, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        if (rarg=="")return a;
        irarg=temposha;
        temposha=a.length();
        std::string product=stringify(atof(larg.c_str())*atof(rarg.c_str()));
        if (larg.find("-")==0)
        {
            if (product.find("-")!=std::string::npos && product.find("-")>1) product="+"+product;
        }
        a.replace(ilarg,irarg-ilarg+1, product);
        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::MULTIPLYORDIVIDE(std::string asdf)
{
    if (asdf.find("/")!=std::string::npos && asdf.find("*")!=std::string::npos)
    {
        if (asdf.find("/")<asdf.find("*"))
        {
            asdf=DIVIDE(asdf);
            asdf=MULTIPLY(asdf);
            return asdf;
        }
        else
        {
            asdf=MULTIPLY(asdf);
            asdf=DIVIDE(asdf);
            return asdf;
        }
    }
    else
    {
        asdf=DIVIDE(asdf);
        asdf=MULTIPLY(asdf);
        return asdf;
    }
}


std::string Eval::POWER(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("^")!=std::string::npos)
    {
        int pluspos=a.find("^");
        int temposha=pluspos;

        while (pluspos!=0 && isdigit(a[temposha-1]))
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0) break;
            temposha--;
        }
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && isdigit(a[temposha+1]))
        {
            rarg += a.substr(temposha+1, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        irarg=temposha;
        temposha=a.length();
        float finalanswer=1;
        int tato=atof(rarg.c_str());
        for (int times=0; times<tato; times++ )
        {
            finalanswer*=atof(larg.c_str());
        }

        a.replace(ilarg,irarg-ilarg+1, stringify(finalanswer));
        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::ADD(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;

    while (a.find("+")!=std::string::npos)
    {
        int pluspos=a.find("+");
        int temposha=pluspos;

        while (pluspos!=0 && (isdigit(a[temposha-1])||a.substr(temposha-1,1)=="-"||a.substr(temposha-1,1)==".") )
        {
            larg = a.substr(temposha-1, 1)+larg;
            if (temposha==0 || a.substr(temposha-1,1)=="-")
            {
                if (temposha!=0) temposha-=1;
                break;

            }
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")return a;
        ilarg=temposha;
        temposha=pluspos;
        while (pluspos!=0 && (isdigit(a[temposha+1])||(a.substr(temposha+1,1)=="-" && temposha==pluspos)||a.substr(temposha+1,1)=="."))
        {
            rarg += a.substr(temposha+1, 1);
            if (temposha==a.length()) break;
            temposha++;
        }
        if (rarg=="")return a;
        irarg=temposha;
        temposha=a.length();
        float sum=atof(larg.c_str())+atof(rarg.c_str());

        a.replace(ilarg,irarg-ilarg+1, stringify(sum));
        larg="";
        rarg="";
    }
    return a;
}



std::string Eval::SUBTRACT(std::string asdf)
{
    std::string larg,rarg;
    std::string a=asdf;
    int ilarg, irarg;
    std::string exceptionss=":?~[]{}&$^";

    while (a.rfind("-")!=std::string::npos&&a.rfind("-")!=0 &&exceptionss.find(a.substr(a.rfind("-")-1, 1))==std::string::npos)
    {
        int pluspos;
        pluspos=a.find("-");

        if (a.find("-")==0||exceptionss.find(a.substr(pluspos-1, 1))!=std::string::npos)pluspos=a.find("-", pluspos+1);

        int temposha=pluspos;
        while (a.rfind("-")!=0 && (isdigit(a[temposha-1])||(a.substr(temposha-1,1)=="-"&&temposha!=0)||a.substr(temposha-1,1)=="."))
        {
            larg = a.substr(temposha-1, 1)+larg;
            temposha--;
            if (temposha==0) break;
        }
        if (larg=="")return a;
        ilarg=temposha;
        temposha=pluspos;
        while (a.rfind("-")!=0 && (isdigit(a[temposha+1])||(a.substr(temposha+1,1)=="-"&&temposha==1)||a.substr(temposha+1,1)=="."))
        {
            rarg += a.substr(temposha+1, 1);
            temposha++;
            if (temposha==a.length()) break;
        }
        if (rarg=="")return a;
        irarg=temposha;
        temposha=a.length();

        a.replace(ilarg,irarg-ilarg+1, stringify((atof(larg.c_str())-atof(rarg.c_str()))));
        larg="";
        rarg="";
    }
    return a;
}

std::string Eval::ADDORSUBTRACT(std::string asdf)
{
    if (asdf.find("-")!=std::string::npos && asdf.find("+")!=std::string::npos)
    {
        if (asdf.find("-")<asdf.find("+"))
        {
            asdf=SUBTRACT(asdf);
            asdf=ADD(asdf);
            return asdf;
        }
        else
        {
            asdf=ADD(asdf);
            asdf=SUBTRACT(asdf);
            return asdf;
        }
    }
    else
    {
        asdf=ADD(asdf);
        asdf=SUBTRACT(asdf);
        return asdf;
    }
}


float Evaluate(std::string strvalue)
{

	if (strvalue.find_first_of("+-/*")!=std::string::npos)
	{
		Eval Calc(strvalue);
		if (Calc.operation.find("inf")!=std::string::npos)
		{
			if (Calc.operation.find("inf")!=0 && Calc.operation[Calc.operation.find("inf")-1]=='-')
				return -1./0.;
			else
				return 1./0.;
		}
		return atof(Calc.operation.c_str());
	}
	else
	{
		if (strvalue.find("inf")!=std::string::npos)
		{
			if (strvalue.find("inf")!=0 && strvalue[strvalue.find("inf")-1]=='-')
				return -1./0.;
			else
				return 1./0.;
		}
		return atof(strvalue.c_str());
	}
    
}

int iEvaluate(std::string strvalue)
{
    Eval Calc(strvalue);
    return atoi(Calc.operation.c_str());
}

std::string stringify(int whatever)
{
    std::ostringstream result;
    result << whatever;
	return result.str();
    
}


std::string stringify(float whatever)
{
	std::ostringstream result;
    result << whatever;
    return result.str();
}

std::string stringify(double whatever)
{
	std::ostringstream result;
    result << whatever;
    return result.str();
}

bool BEvaluate(std::string strvalue)
{
    Eval Calc("0");
    Calc.BEval(strvalue);
    if (atoi(Calc.operation.c_str())==1)
    {
        return true;
    }
    else
    {
        return false;
    }

}

std::string Evaluayh(std::string strvalue)
{
    Eval Calc(strvalue);
    return Calc.operation;
}



