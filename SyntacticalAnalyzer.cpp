/********************************************************************************/
/* Project: 									*/
/* Author: 									*/
/* Date: 									*/
/* Description:									*/
/********************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	lex = new LexicalAnalyzer (filename);
	int fnlength = strlen (filename);
	filename[fnlength-2] = 'p';
	filename[fnlength-1] = '2';
	p2file.open (filename);
	token = lex->GetToken();
	int errors = program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	delete lex;
}

int SyntacticalAnalyzer::program() {
    lex->debug << "program function called\n";
    p2file << "Starting <program>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    // The first token must be an LPAREN_T
    if (token == LPAREN_T) {
        p2file << "Using rule 1\n";
        errors += define();
        //errors += more_defines();

        if((token = lex->GetToken()) != EOF_T) {
            errors++;
        }

    } else {
        errors++;
    }

    p2file << "Ending <program>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::define() {
    lex->debug << "define function called\n";
    p2file << "Starting <define>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;


    if((token = lex->GetToken()) != DEFINE_T) {
        errors++;
    } else {
        p2file << "Using rule 2\n";

        if((token = lex->GetToken()) != LPAREN_T) errors++;

        if((token = lex->GetToken()) != IDENT_T)  errors++;

        errors += param_list();

        std::cout << lex->GetTokenName(token) << std::endl;

        if((token = lex->GetToken()) != RPAREN_T) errors++;

        errors += stmt();
        errors += stmt_list();

        if((token = lex->GetToken()) != RPAREN_T) errors++;
    }


    p2file << "Ending <define>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::stmt() {
    lex->debug << "stmt function called\n";
    p2file << "Starting <stmt>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token) {

        case NUMLIT_T: // Rule 7
            p2file << "Using rule 7\n";
            errors += literal();
            break;

        case IDENT_T: // Rule 8
            p2file << "Using rule 8\n";
            token = lex->GetToken();
            break;

        case LPAREN_T: // Rule 9
            p2file << "Using rule 9\n";
            
            token = lex->GetToken();
            errors += action();

            if((token = lex->GetToken()) != RPAREN_T) errors++;

            break;
        // case QUOTE_T: // Rule 11
        //     p2file << "Using rule 11\n";
        //     errors += quoted_lit();
        //     break;
        
        default:
            // error?
            // loop until firsts or follows of stmt?
            break;
    }

    p2file << "Ending <stmt>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::more_defines() {
    lex->debug << "more_defines function called\n";
    p2file << "Starting <more_defines>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token) {
        case LPAREN_T: // Rule 3
            p2file << "Using rule 3\n";
            errors += define();
            errors += more_defines();
            break;
        case EOF_T: // Rule 4
            //follow (lambda)??
            break;
        default:
            // error?
            // loop until firsts or follows of more_defines?
            break;
    }

    p2file << "Ending <more_defines>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::stmt_list() {
    lex->debug << "stmt_list function called\n";
    p2file << "Starting <stmt_list>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token){
        case RPAREN_T: // Rule 6
            //follow (lambda)??
            break;
        case IDENT_T: // Rule 5
            p2file << "Using rule 5\n";
            errors += stmt();
            errors += stmt_list();
            break;
        case LPAREN_T: // Rule 5
            p2file << "Using rule 5\n";
            errors += stmt();
            errors += stmt_list();
            break;
        case NUMLIT_T: // Rule 5
            p2file << "Using rule 5\n";
            errors += stmt();
            errors += stmt_list();
            break;
        case QUOTE_T: // Rule 5
            p2file << "Using rule 5\n";
            errors += stmt();
            errors += stmt_list();
            break;
        default:
            // error?
            // loop until firsts or follows of stmt_list?
            break;
    }

    p2file << "Ending <stmt_list>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::literal() {
    lex->debug << "literal function called\n";
    p2file << "Starting <literal>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token){
        case NUMLIT_T: // Rule 10
            p2file << "Using rule 10\n";
            token = lex->GetToken();
            break;
        case QUOTE_T: // Rule 11
            p2file << "Using rule 11\n";
            token = lex->GetToken();
            errors += quoted_lit();
            break;
        default:
            // error?
            // loop until firsts or follows of literal?
            break;
    }

    p2file << "Ending <literal>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::quoted_lit() {
    lex->debug << "quoted_lit function called\n";
    p2file << "Starting <quoted_lit>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token){
        case LPAREN_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case IDENT_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case NUMLIT_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case CONS_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case IF_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case DISPLAY_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case NEWLINE_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case LISTOP_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case AND_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case OR_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case NOT_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case DEFINE_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case NUMBERP_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case SYMBOLP_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case LISTP_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case ZEROP_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case NULLP_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case CHARP_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case STRINGP_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case PLUS_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case MINUS_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case DIV_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case MULT_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case EQUALTO_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case GT_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case LT_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case GTE_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case LTE_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        case QUOTE_T: // Rule 12
            p2file << "Using rule 12\n";
            errors += any_other_token();
            break;
        default:
            // loop until firsts or follows of quoted_lit?
            errors++;
            break;
    }

    p2file << "Ending <quoted_lit>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::more_tokens() {
    lex->debug << "more_tokens function called\n";
    p2file << "Starting <more_tokens>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token){
        case RPAREN_T: // Rule 14
            //follow (lambda)??
            break;
        case LPAREN_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case IDENT_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case NUMLIT_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case CONS_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case IF_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case DISPLAY_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case NEWLINE_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case LISTOP_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case AND_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case OR_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case NOT_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case DEFINE_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case NUMBERP_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case SYMBOLP_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case LISTP_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case ZEROP_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case NULLP_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case CHARP_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case STRINGP_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case PLUS_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case MINUS_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case DIV_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case MULT_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case EQUALTO_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case GT_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case LT_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case GTE_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case LTE_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        case QUOTE_T: // Rule 13
            p2file << "Using rule 13\n";
            errors += any_other_token();
            errors += more_tokens();
            break;
        default:
            // error?
            // loop until firsts or follows of more_tokens?
            break;
    }

    p2file << "Ending <more_tokens>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::param_list() {
    lex->debug << "param_list function called\n";
    p2file << "Starting <param_list>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token){
        case IDENT_T: // Rule 15
            p2file << "Using rule 15\n";
            token = lex->GetToken();
            errors += param_list();
            break;
        case RPAREN_T: // Rule 16
            //follow (lambda)??
            break;
        default:
            // error?
            // loop until firsts or follows of param_list?
            break;
    }

    p2file << "Ending <param_list>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::else_part() {
    lex->debug << "else_part function called\n";
    p2file << "Starting <else_part>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token){
        case IDENT_T: // Rule 17
            p2file << "Using rule 17\n";
            errors += stmt();
            break;
        case LPAREN_T: // Rule 17
            p2file << "Using rule 17\n";
            errors += stmt();
            break;
        case RPAREN_T: // Rule 18
            //follow (lambda)??
            break;
        case NUMLIT_T: // Rule 17
            p2file << "Using rule 17\n";
            errors += stmt();
            break;
        case QUOTE_T: // Rule 17
            p2file << "Using rule 17\n";
            errors += stmt();
            break;
        default:
            // error?
            // loop until firsts or follows of else_part?
            break;
    }

    p2file << "Ending <else_part>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::action() {
    lex->debug << "action function called\n";
    p2file << "Starting <action>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token){
        case IF_T: // Rule 19
            p2file << "Using rule 19\n";
            token = lex->GetToken();
            errors += stmt();
            errors += stmt();
            errors += else_part();
            break;
        case LISTOP_T: // Rule 20
            p2file << "Using rule 20\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case CONS_T: // Rule 21
            p2file << "Using rule 21\n";
            token = lex->GetToken();
            errors += stmt();
            errors += stmt();
            break;
        case AND_T: // Rule 22
            p2file << "Using rule 22\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case OR_T: // Rule 23
            p2file << "Using rule 23\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case NOT_T: // Rule 24
            p2file << "Using rule 24\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case NUMBERP_T: // Rule 25
            p2file << "Using rule 25\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case SYMBOLP_T: // Rule 26
            p2file << "Using rule 26\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case LISTP_T: // Rule 27
            p2file << "Using rule 27\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case ZEROP_T: // Rule 28
            p2file << "Using rule 28\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case NULLP_T: // Rule 29
            p2file << "Using rule 29\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case CHARP_T: // Rule 30
            p2file << "Using rule 30\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case STRINGP_T: // Rule 31
            p2file << "Using rule 31\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case PLUS_T: // Rule 32
            p2file << "Using rule 32\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case MINUS_T: // Rule 33
            p2file << "Using rule 33\n";
            token = lex->GetToken();
            errors += stmt();
            errors += stmt_list();
            break;
        case DIV_T: // Rule 34
            p2file << "Using rule 34\n";
            token = lex->GetToken();
            errors += stmt();
            errors += stmt_list();
            break;
        case MULT_T: // Rule 35
            p2file << "Using rule 35\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case EQUALTO_T: // Rule 36
            p2file << "Using rule 36\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case GT_T: // Rule 37
            p2file << "Using rule 37\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case LT_T: // Rule 38
            p2file << "Using rule 38\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case GTE_T: // Rule 39
            p2file << "Using rule 39\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case LTE_T: // Rule 40
            p2file << "Using rule 40\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case IDENT_T: // Rule 41
            p2file << "Using rule 41\n";
            token = lex->GetToken();
            errors += stmt_list();
            break;
        case DISPLAY_T: // Rule 42
            p2file << "Using rule 42\n";
            token = lex->GetToken();
            errors += stmt();
            break;
        case NEWLINE_T: // Rule 43
            p2file << "Using rule 43\n";
            token = lex->GetToken();
            break;
        default:
            // error?
            // loop until firsts or follows of action?
            break;
    }

    p2file << "Ending <action>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}

int SyntacticalAnalyzer::any_other_token() {
    lex->debug << "any_other_token function called\n";
    p2file << "Starting <any_other_token>. Current token = " << lex->GetTokenName(token) << ".\n";
    int errors = 0;

    switch(token){
        case LPAREN_T: // Rule 44
            p2file << "Using rule 44\n";
            token = lex->GetToken();
            errors += more_tokens();
            if((token = lex->GetToken()) != RPAREN_T){
                errors++;
                // error?
            }
            break;
        case IDENT_T: // Rule 45
            p2file << "Using rule 45\n";
            token = lex->GetToken();
            break;
        case NUMLIT_T: // Rule 46
            p2file << "Using rule 46\n";
            token = lex->GetToken();
            break;
        case CONS_T: // Rule 47
            p2file << "Using rule 47\n";
            token = lex->GetToken();
            break;
        case IF_T: // Rule 48
            p2file << "Using rule 48\n";
            token = lex->GetToken();
            break;
        case DISPLAY_T: // Rule 49
            p2file << "Using rule 49\n";
            token = lex->GetToken();
            break;
        case NEWLINE_T: // Rule 50
            p2file << "Using rule 50\n";
            token = lex->GetToken();
            break;
        case LISTOP_T: // Rule 51
            p2file << "Using rule 51\n";
            token = lex->GetToken();
            break;
        case AND_T: // Rule 52
            p2file << "Using rule 52\n";
            token = lex->GetToken();
            break;
        case OR_T: // Rule 53
            p2file << "Using rule 53\n";
            token = lex->GetToken();
            break;
        case NOT_T: // Rule 54
            p2file << "Using rule 54\n";
            token = lex->GetToken();
            break;
        case DEFINE_T: // Rule 55
            p2file << "Using rule 55\n";
            token = lex->GetToken();
            break;
        case NUMBERP_T: // Rule 56
            p2file << "Using rule 56\n";
            token = lex->GetToken();
            break;
        case SYMBOLP_T: // Rule 57
            p2file << "Using rule 57\n";
            token = lex->GetToken();
            break;
        case LISTP_T: // Rule 58
            p2file << "Using rule 58\n";
            token = lex->GetToken();
            break;
        case ZEROP_T: // Rule 59
            p2file << "Using rule 59\n";
            token = lex->GetToken();
            break;
        case NULLP_T: // Rule 60
            p2file << "Using rule 60\n";
            token = lex->GetToken();
            break;
        case CHARP_T: // Rule 61
            p2file << "Using rule 61\n";
            token = lex->GetToken();
            break;
        case STRINGP_T: // Rule 62
            p2file << "Using rule 62\n";
            token = lex->GetToken();
            break;
        case PLUS_T: // Rule 63
            p2file << "Using rule 63\n";
            token = lex->GetToken();
            break;
        case MINUS_T: // Rule 64
            p2file << "Using rule 64\n";
            token = lex->GetToken();
            break;
        case DIV_T: // Rule 65
            p2file << "Using rule 65\n";
            token = lex->GetToken();
            break;
        case MULT_T: // Rule 66
            p2file << "Using rule 66\n";
            token = lex->GetToken();
            break;
        case EQUALTO_T: // Rule 67
            p2file << "Using rule 67\n";
            token = lex->GetToken();
            break;
        case GT_T: // Rule 68
            p2file << "Using rule 68\n";
            token = lex->GetToken();
            break;
        case LT_T: // Rule 69
            p2file << "Using rule 69\n";
            token = lex->GetToken();
            break;
        case GTE_T: // Rule 70
            p2file << "Using rule 70\n";
            token = lex->GetToken();
            break;
        case LTE_T: // Rule 71
            p2file << "Using rule 71\n";
            token = lex->GetToken();
            break;
        case QUOTE_T: // Rule 72
            p2file << "Using rule 72\n";
            token = lex->GetToken();
            break;
        default:
            errors++;
            // loop until firsts or follows of any_other_token?
            break;
    }

    p2file << "Ending <any_other_token>. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
    return errors;
}



