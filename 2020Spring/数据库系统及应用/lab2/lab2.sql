/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2020/4/29 9:06:39                            */
/*==============================================================*/


alter table bankCustomer 
   drop foreign key FK_BANKCUST_BANKCUSTO_BANKSUBB;

alter table bankCustomer 
   drop foreign key FK_BANKCUST_BANKCUSTO_CUSTOMER;

alter table cheque 
   drop foreign key FK_CHEQUE_CHEQUEBAN_BANKSUBB;

alter table chequeCustomer 
   drop foreign key FK_CHEQUECU_CHEQUECUS_CUSTOMER;

alter table chequeCustomer 
   drop foreign key FK_CHEQUECU_CHEQUECUS_CHEQUE;

alter table customer 
   drop foreign key FK_CUSTOMER_TAKECHARG_STAFF;

alter table customerLoan 
   drop foreign key FK_CUSTOMER_CUSTOMERL_LOAN;

alter table customerLoan 
   drop foreign key FK_CUSTOMER_CUSTOMERL_CUSTOMER;

alter table department 
   drop foreign key FK_DEPARTME_BANKDEPAR_BANKSUBB;

alter table department 
   drop foreign key FK_DEPARTME_LEAD_STAFF;

alter table loan 
   drop foreign key FK_LOAN_BANKLOAN_BANKSUBB;

alter table payment 
   drop foreign key FK_PAYMENT_PAY_LOAN;

alter table save 
   drop foreign key FK_SAVE_SAVEBANK_BANKSUBB;

alter table staff 
   drop foreign key FK_STAFF_BELONG_DEPARTME;

alter table svaeCustomer 
   drop foreign key FK_SVAECUST_SVAECUSTO_CUSTOMER;

alter table svaeCustomer 
   drop foreign key FK_SVAECUST_SVAECUSTO_SAVE;


alter table bankCustomer 
   drop foreign key FK_BANKCUST_BANKCUSTO_BANKSUBB;

alter table bankCustomer 
   drop foreign key FK_BANKCUST_BANKCUSTO_CUSTOMER;

drop table if exists bankCustomer;

drop table if exists bankSubbranch;


alter table cheque 
   drop foreign key FK_CHEQUE_CHEQUEBAN_BANKSUBB;

drop table if exists cheque;


alter table chequeCustomer 
   drop foreign key FK_CHEQUECU_CHEQUECUS_CUSTOMER;

alter table chequeCustomer 
   drop foreign key FK_CHEQUECU_CHEQUECUS_CHEQUE;

drop table if exists chequeCustomer;


alter table customer 
   drop foreign key FK_CUSTOMER_TAKECHARG_STAFF;

drop table if exists customer;


alter table customerLoan 
   drop foreign key FK_CUSTOMER_CUSTOMERL_LOAN;

alter table customerLoan 
   drop foreign key FK_CUSTOMER_CUSTOMERL_CUSTOMER;

drop table if exists customerLoan;


alter table department 
   drop foreign key FK_DEPARTME_BANKDEPAR_BANKSUBB;

alter table department 
   drop foreign key FK_DEPARTME_LEAD_STAFF;

drop table if exists department;


alter table loan 
   drop foreign key FK_LOAN_BANKLOAN_BANKSUBB;

drop table if exists loan;


alter table payment 
   drop foreign key FK_PAYMENT_PAY_LOAN;

drop table if exists payment;


alter table save 
   drop foreign key FK_SAVE_SAVEBANK_BANKSUBB;

drop table if exists save;


alter table staff 
   drop foreign key FK_STAFF_BELONG_DEPARTME;

drop table if exists staff;


alter table svaeCustomer 
   drop foreign key FK_SVAECUST_SVAECUSTO_CUSTOMER;

alter table svaeCustomer 
   drop foreign key FK_SVAECUST_SVAECUSTO_SAVE;

drop table if exists svaeCustomer;

/*==============================================================*/
/* Table: bankCustomer                                          */
/*==============================================================*/
create table bankCustomer
(
   bankName             varchar(256) not null  comment 'bankName',
   customerID           varchar(18) not null  comment 'customerID',
   primary key (bankName, customerID)
);

alter table bankCustomer comment 'bankCustomer';

/*==============================================================*/
/* Table: bankSubbranch                                         */
/*==============================================================*/
create table bankSubbranch
(
   bankName             varchar(256) not null  comment 'bankName',
   city                 varchar(256)  comment 'city',
   property             numeric(16,2) not null  comment 'property',
   primary key (bankName)
);

alter table bankSubbranch comment '支行';

/*==============================================================*/
/* Table: cheque                                                */
/*==============================================================*/
create table cheque
(
   accountID            varchar(36) not null  comment 'accountID',
   bankName             varchar(256) not null  comment 'bankName',
   balance              numeric(16,2)  comment 'balance',
   openDate             date  comment 'openDate',
   type                 varchar(36)  comment 'type',
   visitDate            date  comment 'visitDate',
   overdraft            numeric(16,2) not null  comment 'overdraft',
   primary key (accountID)
);

alter table cheque comment 'cheque';

/*==============================================================*/
/* Table: chequeCustomer                                        */
/*==============================================================*/
create table chequeCustomer
(
   customerID           varchar(18) not null  comment 'customerID',
   accountID            varchar(36) not null  comment 'accountID',
   primary key (customerID, accountID)
);

alter table chequeCustomer comment 'chequeCustomer';

/*==============================================================*/
/* Table: customer                                              */
/*==============================================================*/
create table customer
(
   customerID           varchar(18) not null  comment 'customerID',
   staffID              varchar(18)  comment 'staffID',
   address              varchar(256)  comment 'address',
   customerName         varchar(18) not null  comment 'customerName',
   phone                varchar(11)  comment 'phone',
   linkmanEmail         varchar(64) not null  comment 'linkmanEmail',
   linkmanName          varchar(18) not null  comment 'linkmanName',
   linkmanPhone         varchar(11)  comment 'linkmanPhone',
   linkmanRelationship  varchar(64)  comment 'linkmanRelationship',
   primary key (customerID)
);

alter table customer comment 'customer';

/*==============================================================*/
/* Table: customerLoan                                          */
/*==============================================================*/
create table customerLoan
(
   loanID               varchar(18) not null  comment 'loanID',
   customerID           varchar(18) not null  comment 'customerID',
   primary key (loanID, customerID)
);

alter table customerLoan comment 'customerLoan';

/*==============================================================*/
/* Table: department                                            */
/*==============================================================*/
create table department
(
   departmentNum        varchar(18) not null  comment 'departmentNum',
   bankName             varchar(256)  comment 'bankName',
   staffID              varchar(18)  comment 'staffID',
   departmentName       varchar(18)  comment 'departmentName',
   type                 int  comment 'type',
   primary key (departmentNum)
);

alter table department comment 'department';

/*==============================================================*/
/* Table: loan                                                  */
/*==============================================================*/
create table loan
(
   loanID               varchar(18) not null  comment 'loanID',
   bankName             varchar(256) not null  comment 'bankName',
   sum                  numeric(16,2) not null  comment 'sum',
   times                int  comment 'times',
   paySum               numeric(16,2) not null  comment 'paySum',
   primary key (loanID)
);

alter table loan comment 'loan';

/*==============================================================*/
/* Table: payment                                               */
/*==============================================================*/
create table payment
(
   loanID               varchar(18) not null  comment 'loanID',
   payDate              date not null  comment 'payDate',
   cash                 numeric(16,2) not null  comment 'cash',
   primary key (loanID, payDate)
);

alter table payment comment 'payment';

/*==============================================================*/
/* Table: save                                                  */
/*==============================================================*/
create table save
(
   accountID            varchar(36) not null  comment 'accountID',
   bankName             varchar(256) not null  comment 'bankName',
   balance              numeric(16,2)  comment 'balance',
   openDate             date  comment 'openDate',
   type                 varchar(36)  comment 'type',
   visitDate            date  comment 'visitDate',
   rate                 float not null  comment 'rate',
   currency             varchar(24) not null  comment 'currency',
   primary key (accountID)
);

alter table save comment 'save';

/*==============================================================*/
/* Table: staff                                                 */
/*==============================================================*/
create table staff
(
   staffID              varchar(18) not null  comment 'staffID',
   departmentNum        varchar(18)  comment 'departmentNum',
   startDate            date not null  comment 'startDate',
   address              varchar(256)  comment 'address',
   staffName            varchar(18) not null  comment 'staffName',
   phone                varchar(11)  comment 'phone',
   primary key (staffID)
);

alter table staff comment 'staff';

/*==============================================================*/
/* Table: svaeCustomer                                          */
/*==============================================================*/
create table svaeCustomer
(
   customerID           varchar(18) not null  comment 'customerID',
   accountID            varchar(36) not null  comment 'accountID',
   primary key (customerID, accountID)
);

alter table svaeCustomer comment 'svaeCustomer';

alter table bankCustomer add constraint FK_BANKCUST_BANKCUSTO_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete restrict on update restrict;

alter table bankCustomer add constraint FK_BANKCUST_BANKCUSTO_CUSTOMER foreign key (customerID)
      references customer (customerID) on delete restrict on update restrict;

alter table cheque add constraint FK_CHEQUE_CHEQUEBAN_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete restrict on update restrict;

alter table chequeCustomer add constraint FK_CHEQUECU_CHEQUECUS_CUSTOMER foreign key (customerID)
      references customer (customerID) on delete restrict on update restrict;

alter table chequeCustomer add constraint FK_CHEQUECU_CHEQUECUS_CHEQUE foreign key (accountID)
      references cheque (accountID) on delete restrict on update restrict;

alter table customer add constraint FK_CUSTOMER_TAKECHARG_STAFF foreign key (staffID)
      references staff (staffID) on delete restrict on update restrict;

alter table customerLoan add constraint FK_CUSTOMER_CUSTOMERL_LOAN foreign key (loanID)
      references loan (loanID) on delete restrict on update restrict;

alter table customerLoan add constraint FK_CUSTOMER_CUSTOMERL_CUSTOMER foreign key (customerID)
      references customer (customerID) on delete restrict on update restrict;

alter table department add constraint FK_DEPARTME_BANKDEPAR_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete restrict on update restrict;

alter table department add constraint FK_DEPARTME_LEAD_STAFF foreign key (staffID)
      references staff (staffID) on delete restrict on update restrict;

alter table loan add constraint FK_LOAN_BANKLOAN_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete restrict on update restrict;

alter table payment add constraint FK_PAYMENT_PAY_LOAN foreign key (loanID)
      references loan (loanID) on delete restrict on update restrict;

alter table save add constraint FK_SAVE_SAVEBANK_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete restrict on update restrict;

alter table staff add constraint FK_STAFF_BELONG_DEPARTME foreign key (departmentNum)
      references department (departmentNum) on delete restrict on update restrict;

alter table svaeCustomer add constraint FK_SVAECUST_SVAECUSTO_CUSTOMER foreign key (customerID)
      references customer (customerID) on delete restrict on update restrict;

alter table svaeCustomer add constraint FK_SVAECUST_SVAECUSTO_SAVE foreign key (accountID)
      references save (accountID) on delete restrict on update restrict;

