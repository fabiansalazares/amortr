#include <Rcpp.h>
using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//

Rcpp::DateVector generate_date_range(
    Date start_date,  // date in which the loan is taken out, i.e. when the borrower receives the principal amount.
    Nullable<Date> end_date = R_NilValue, // final date of the loan
    Nullable<NumericVector> n_payments = R_NilValue, // number of periods of the loan, i.e. number of payments from borrower to lender
    int frequency = 12, // number of payments done in every year
    bool truncate_month_day = true // trunk month payment_date to the first day of the month
) {
  
  int total_periods;
  
  // end_date passed. end_date overrides n_payments if both arguments are not NULL.
  if((end_date.isNotNull() &&  n_payments.isNull()) || (end_date.isNotNull() && n_payments.isNotNull())) {
    Date end_date_(end_date); //casting nullable Date to Date
    int start_date_year;
    int start_date_month;
    int end_date_year;
    int end_date_month;
    
    end_date_year = end_date_.getYear();
    end_date_month = end_date_.getMonth();
    start_date_year = start_date.getYear();
    start_date_month = start_date.getMonth();
    
    total_periods = ((end_date_year - start_date_year )*12 - (end_date_month - start_date_month)) / (12/frequency);
    
    if(total_periods < 0) {
      stop("End date must be after the start date.");
    }

    // total_periods passed
  } else if (end_date.isNull() &&  n_payments.isNotNull()) {
    total_periods = int(NumericVector(n_payments)[0]); // casting to underlying type NumericVector and then accessing first value
  } else {
    stop("Either end_date or n_periods must take a non NULL value.");
  }
  
  DateVector date_range(total_periods);
  
  
  if(truncate_month_day) {
    date_range[0] = Date(
      Date(start_date).getYear(), // year
      Date(start_date).getMonth(), // month
      (1)
    );
    
    int months_to_add = 12/frequency;
    int month_value_after_adding;
    int month_value_to_set;
    int year_value_to_set;
    Date previous_date;
    
    for(int i=1; i<total_periods; i++) {
      previous_date = Date(date_range[i-1]);
      month_value_after_adding = previous_date.getMonth() + months_to_add;
      
      if(month_value_after_adding > 12) {
        month_value_to_set = month_value_after_adding % 12;
        year_value_to_set = previous_date.getYear() + 1;
      } else {
        month_value_to_set = month_value_after_adding;
        year_value_to_set = previous_date.getYear();
      };
      
      date_range[i] = Date(
        year_value_to_set,
        month_value_to_set,
        1
      );
      
    }
  } else {
    // NOT truncating the day of the month to 01
    int initial_day = Date(start_date).getDay();
    
    date_range[0] = Date(
      Date(start_date).getYear(), // year
      Date(start_date).getMonth(), // month
      initial_day                  // day
    );
    
    int months_to_add = 12/frequency;
    int month_value_after_adding;
    int day_value_to_set;
    int month_value_to_set;
    int year_value_to_set;
    Date previous_date;
    
    if (initial_day > 28) {
      for(int i=1; i<total_periods; i++) {
        previous_date = Date(date_range[i-1]);
        month_value_after_adding = previous_date.getMonth() + months_to_add;
        
        if(month_value_after_adding > 12) {
          month_value_to_set = month_value_after_adding % 12;
          year_value_to_set = previous_date.getYear() + 1;
        } else {
          month_value_to_set = month_value_after_adding;
          year_value_to_set = previous_date.getYear();
        };
        
        if(month_value_to_set == 2) {
          day_value_to_set=28;
        } else if (initial_day == 31 && 
          (month_value_to_set == 4 || 
          month_value_to_set == 6 ||
          month_value_to_set == 9 ||
          month_value_to_set == 11)){
          day_value_to_set=30;
        } else {
          day_value_to_set=initial_day;
        }
        
        date_range[i] = Date(
          year_value_to_set,
          month_value_to_set,
          day_value_to_set
        );
      }
      
    } else {
      Rcout << "not truncating, < 28" << std::endl;
      
      // start_date day is equal to or less than 28

      for(int i=1; i<total_periods; i++) {
        previous_date = Date(date_range[i-1]);
        month_value_after_adding = previous_date.getMonth() + months_to_add;
        
        if(month_value_after_adding > 12) {
          month_value_to_set = month_value_after_adding % 12;
          year_value_to_set = previous_date.getYear() + 1;
        } else {
          month_value_to_set = month_value_after_adding;
          year_value_to_set = previous_date.getYear();
        };
        
        date_range[i] = Date(
          year_value_to_set,
          month_value_to_set,
          initial_day
        );
      }
      
    }
  }
  
  return date_range;
  
}