#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

// need to be declared because used in final_check
void m_move (const char, std::string);

std::string get_date(){

    time_t t = time(0); // get current time
    struct tm* now = localtime(&t);
    char date[11]; // somehow 11 chars are needed although there are only 10 used (xx/xx/xxx)

    strftime(date, sizeof(date), "%d/%m/%Y" ,now);
    return date;

}

unsigned int ctoi (const char c){

    int i = c - 48;
    return i;

}

char itoc (const unsigned int i){

    char c = i + 48;
    return c;

}

std::string itos (const int i){

    std::stringstream ss;
    ss << i;
    std::string i_str = ss.str();
    return i_str;

}


bool valid_date(std::string date){

    std::stringstream check(date);
    int d,m,y;

    check >> d;
    check.ignore(1);
    check >> m;
    check.ignore(1);
    check >> y;

    if (d <= 31){
        if (m <= 12){
            if (y >= 1996 && y <= 2096)
                return true;
        }
    }
    return false;

}

bool valid_am(std::string amount, std::string currency){

    bool cor_amount = false;
    bool cor_cur = false;

    std::stringstream check(amount);
    int get_ints;
    check >> get_ints;

    if(check.eof())
        cor_amount = true;

    if (!(currency == "UNVALID"))
        cor_cur = true;

    if(cor_amount && cor_cur)
        return true;
    return false;

}

void format_am(std::string& amount){

    if (amount.length() == 1)
        amount.insert(0, "0.0");
    else if (amount.length() == 2)
        amount.insert(0, "0.");
    else
        amount.insert(amount.length() - 2, ".");

}

void reformat_am(std::string& amount){

    amount.erase(amount.length() - 4, 1);

    // get rid of additional zeros
    for(unsigned int i = 0; i <= amount.length(); ++i){
        if (ctoi(amount[i]) > 0)
            break;
        else
            amount.erase(i,1);
    }

}

void final_check (const char* where, std::string* file_name, std::string* date, std::string* amount, std::string* currency, std::string* remark){

    // DONE
    bool final_check = false;
    while (final_check == false){

        format_am(*amount);
        std::cout << "\nYou gave following data concerning EXPENSES: ";
        std::cout << "\nDate: " << *date;
        std::cout << "\nAmount: " << *amount << " " << *currency;
        std::cout << "\nRemark: " << *remark;

        char final_check_answer;
        std::cout << "\nIs everything correct? (y/n) ";
        std::cin >> final_check_answer;
        if (final_check_answer == 'y')
            final_check = true;
        else if (final_check_answer == 'n'){
            std::cout << "Correct your input: \n";
            m_move(*where, *file_name);
            return;
        }
        else
            std::cout << "No valid answer. Returning to the beginning. \n";

    }

    std::cout << "\nUsing file: " << *file_name << "\n";

    unsigned int am_width = 10;
    // ::app stands appending, so the text is written at the end of the file
    std::ofstream w_file ((*file_name).c_str(), std::ios::app);

    if(w_file.is_open()){
        w_file << *date << " | ";

        if (*where == 'O'){
            w_file << "- ";
            am_width -= 2;
        }

        w_file.width(am_width);
        w_file << *amount << " " << *currency;
        w_file << " | " << *remark << "\n";
    }
    else {
        std::cout << "Unable to open file. \n";
    }
}
// TO DO
void ex_rate(std::string action){
    // "upd" when need to update the exchange rates
    // "rd" to display a table of exchange rates

    // -> store in .txt file?

    std::cout << "Sorry, no databases included yet, exchange rates need to be changes manually.";
    std::cout << "(Which means going through the source code and changing the variables.) \n";

}

unsigned int inascii (const char c){
    unsigned int i = c;
    return i;
}

// PRE: give a amount in original_cur (in cents, rappen, ...)
// POST: returns the given amount in new_cur (in cents, rappen, ...)
double cur_change (double amount, std::string original_cur, std::string new_cur){

    // create "changecodes" (eg. 6769 for CHF to EUR)
    // CHF = 67 - EUR = 69
    std::stringstream ss;
    ss << inascii(original_cur[0]) << inascii(new_cur[0]);
    unsigned int from_to;
    ss >> from_to;

    double rate;
    switch (from_to){
    case 6967:
        rate = 1.05;
        break;
    case 6769:
        rate = 1/1.05;
        break;
    }

    return amount*rate;


}

void m_move(const char where, std::string file_name){

    std::string date, amount, currency, remark;
    std::cout << "\n - For standard values simply type <#> - \n";

        bool cor_date = false;
        while(!cor_date){

            std::cout << "\nDate > ";
            std::cin >> date;

            if (date == "#"){
                date = get_date();
                cor_date = true;
            }
            else {
                if (valid_date(date))
                    cor_date = true;
                else
                    std::cout << "Not a valid date.";
            }
        }

        bool cor_am = false;
        while (!cor_am){

            std::cout << "Amount > ";
            std::cin >> amount;

            if (amount == "#"){
                amount = "0";
            }

            std::cout << "Currency > ";
            std::cin >> currency;

            if (currency == "c" || currency == "#")
                currency = "CHF";
            else if (currency == "e")
                currency = "EUR";
            else
                currency = "UNVALID";

            if(valid_am(amount, currency))
                cor_am = true;
            else
                std::cout << "Not a valid amount (in cent or rappen) or currency (<E>UR or <C>HF).\n";
            }

        bool cor_rem = false;
        while (!cor_rem){ // actually while(true) would also work bc of the breaks & continues but its more obvious this way

            char check_rem;

            std::cout << "Remark >> \n";
            // erase \n still left in the cin
            std::string sink;
            std::getline (std::cin, sink);
            std::getline (std::cin, remark);

            if (remark == "#"){
                remark = "";
                std::cout << "\nNo remark? <y/n> ";
                std::cin >> check_rem;

                if (check_rem == 'y')
                    break;
                else if (check_rem == 'n'){
                    std::cout << "Change the ";
                    continue;
                }
                else {
                    std::cout << "No valid anser. Returning to begin. \n";
                    continue;
                }
            }

            std::cout << "\nLeave this: \n" << remark << "\nas remark? <y/n> ";
            std::cin >> check_rem;

            if (check_rem == 'y')
                break;
            else if (check_rem == 'n'){
                std::cout << "Change the ";
                continue;
            }
            else {
                std::cout << "No valid anser. Returning to begin. \n";
                continue;
            }
        }
    final_check(&where, &file_name, &date, &amount, &currency, &remark);
}

// - display currency when displaying amount
void m_calc(std::string* file_name){

    int sum = 0;
    std::ifstream r_file((*file_name).c_str());

    if (r_file.is_open()){
        std::string current_am, current_line, sink;
        char current_cur;
        int current_val;
        int val_CHF = 0;
        int val_EUR = 0;
        while(std::getline(r_file, current_line)){

            std::stringstream cur_line_ss (current_line);

            // put date in sink
            std::getline(cur_line_ss, sink, '|');
            // get amount string
            std::getline(cur_line_ss, current_am, '|');
            // put remark in sink
            std::getline(cur_line_ss, sink);

            // get currency
            current_cur = current_am[current_am.length() - 4];

            // get sign
            std::string current_sgn;
            if(current_am[1] == '-')
                current_sgn = "NEG";
            else
                current_sgn = "POS";

            current_am.erase(0,2);
            current_am.erase(current_am.length() - 4, 4);

            reformat_am(current_am);
            std::stringstream ss(current_am);
            ss >> current_val;

            if(current_sgn == "NEG")
                current_val = -current_val;

            switch (inascii(current_cur)){
            case 67:
                val_CHF += current_val;
                std::cout << "val_CHF = " << val_CHF << std::endl;
                break;
            case 69:
                val_EUR += current_val;
                std::cout << "val_EUR = " << val_EUR << std::endl;
                break;
            }
        }

        char scur_out;
        std::string lcur_out;

        std::cout << "In which currency? (<C>HF, <E>UR) > ";
        std::cin >> scur_out;

        std::cout << "val_CHF = " << val_CHF << std::endl;
        std::cout << "val_EUR = " << val_EUR << std::endl;

        switch (inascii(scur_out)){
        case 67: case 99: // c = 99
            std::cout << "Sum = " << sum << std::endl;
            sum += val_CHF;
            std::cout << "Sum = " << sum << std::endl;
            if (val_EUR != 0)
                sum += cur_change(val_EUR, "EUR", "CHF");
            lcur_out = "CHF";
            break;
        case 69: case 101: // e = 101
            sum += val_EUR;
            if (val_CHF != 0)
                sum += cur_change(val_CHF, "CHF", "EUR");
            lcur_out = "EUR";
            break;
        }
        std::cout << "CUR CHNG = " << cur_change(val_EUR, "EUR", "CHF") << std::endl;
        std::string sum_str = itos(sum);
        format_am(sum_str);
        std::cout << sum_str << " " << lcur_out << std::endl;
    }
    else
        std::cout << "Unable to open file!";

}

int main(void){

    std::string file_name = "expen.txt";
    char check;

while(true){
    std::cout << "To change the exchange rate(s) enter <X>. \n\n";
    std::cout << "Money in <I> or out <O> or calculate <C> > ";
    std::cin >> check;

    if (check == 'I' || check == 'i')
        m_move('I', file_name);
    if (check == 'O' || check == 'o')
        m_move('O', file_name);
    if (check == 'C' || check == 'c')
        m_calc(&file_name);
    if (check == 'X' || check == 'x')
        ex_rate("upd");

}
    return 0;
}
