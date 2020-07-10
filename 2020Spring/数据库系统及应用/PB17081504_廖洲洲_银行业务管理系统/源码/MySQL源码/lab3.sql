/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2020/6/24 9:06:39                            */
/*==============================================================*/
drop database bankmanagementsystem;
create database bankmanagementsystem default charset=utf8;
use bankmanagementsystem;
SET foreign_key_checks = 0;
drop table if exists bankCustomer;
drop table if exists bankSubbranch;
drop table if exists cheque;
drop table if exists chequeCustomer;
drop table if exists customer;
drop table if exists customerLoan;
drop table if exists department;
drop table if exists loan;
drop table if exists payment;
drop table if exists save;
drop table if exists staff;
drop table if exists saveCustomer;
drop table if exists saveInfo;
drop table if exists chequeInfo;
SET foreign_key_checks = 1;
/*==============================================================*/
/* Table: bankCustomer                                          */
/*==============================================================*/
create table bankCustomer
(
   bankName             varchar(50) not null  comment 'bankName',
   customerID           varchar(18) not null  comment 'customerID',
   accountType          varchar(1) not null comment 'accountType',
   primary key (bankName, customerID,accountType)
);

alter table bankCustomer comment 'bankCustomer';

/*==============================================================*/
/* Table: bankSubbranch                                         */
/*==============================================================*/
create table bankSubbranch
(
   bankName             varchar(50) not null  comment 'bankName',
   city                 varchar(50)  comment 'city',
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
   bankName             varchar(50) not null  comment 'bankName',
   balance              numeric(16,2)  comment 'balance',
   openTime             datetime  comment 'openTime',
   visitTime            datetime  comment 'visitTime',
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
   address              varchar(50)  comment 'address',
   customerName         varchar(18) not null  comment 'customerName',
   createTime           datetime not null comment 'createTime',
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
   bankName             varchar(50)  comment 'bankName',
   manager              varchar(18)  comment 'manager',
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
   bankName             varchar(50) not null  comment 'bankName',
   sum                  numeric(16,2) not null  comment 'sum',
   times                int  comment 'times',
   paySum               numeric(16,2) not null  comment 'paySum',
   state				char(1)			default '0',
   primary key (loanID)
);

alter table loan comment 'loan';

/*==============================================================*/
/* Table: payment                                               */
/*==============================================================*/
create table payment
(
   loanID               varchar(18) not null  comment 'loanID',
   payTime              datetime not null  comment 'payTime',
   cash                 numeric(16,2) not null  comment 'cash',
   primary key (loanID, payTime)
);

alter table payment comment 'payment';

/*==============================================================*/
/* Table: save                                                  */
/*==============================================================*/
create table save
(
   accountID            varchar(36) not null  comment 'accountID',
   bankName             varchar(50) not null  comment 'bankName',
   balance              numeric(16,2)  comment 'balance',
   openTime             datetime  comment 'openTime',
   visitTime            datetime  comment 'visitTime',
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
   address              varchar(50)  comment 'address',
   staffName            varchar(18) not null  comment 'staffName',
   phone                varchar(11)  comment 'phone',
   primary key (staffID)
);

alter table staff comment 'staff';

/*==============================================================*/
/* Table: saveCustomer                                          */
/*==============================================================*/
create table saveCustomer
(
   customerID           varchar(18) not null  comment 'customerID',
   accountID            varchar(36) not null  comment 'accountID',
   primary key (customerID, accountID)
);

# 为了统计业务信息为每个账户增加业务表记录存取款信息
create table saveInfo
(
	accountID		varchar(36) not null comment 'accountID',
    businessType	char(1) not null comment '存取款',
    visitTime		datetime not null comment 'visitTime',
    cash 			numeric(16,2) not null comment 'cash',
    balance         numeric(16,2) not null comment 'balance',
    primary key (accountID,visitTime),
    foreign key(accountID) references save(accountID) On Delete Cascade
);

create table chequeInfo
(
	accountID		varchar(36) not null comment 'accountID',
    businessType	char(1) not null comment '存取款',
    visitTime		datetime not null comment 'visitTime',
    cash 			numeric(16,2) not null comment 'cash',
    balance         numeric(16,2) not null comment 'balance',
    primary key (accountID,visitTime),
    foreign key(accountID) references cheque(accountID) On Delete Cascade
);

alter table saveCustomer comment 'saveCustomer';

alter table bankCustomer add constraint FK_BANKCUST_BANKCUSTO_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete restrict on update cascade;

alter table bankCustomer add constraint FK_BANKCUST_BANKCUSTO_CUSTOMER foreign key (customerID)
      references customer (customerID) on delete cascade on update cascade;

alter table cheque add constraint FK_CHEQUE_CHEQUEBAN_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete restrict on update cascade;

alter table chequeCustomer add constraint FK_CHEQUECU_CHEQUECUS_CUSTOMER foreign key (customerID)
      references customer (customerID) on delete restrict on update restrict;

alter table chequeCustomer add constraint FK_CHEQUECU_CHEQUECUS_CHEQUE foreign key (accountID)
      references cheque (accountID) on delete cascade on update cascade;

alter table customer add constraint FK_CUSTOMER_TAKECHARG_STAFF foreign key (staffID)
      references staff (staffID) on delete restrict on update cascade;

alter table customerLoan add constraint FK_CUSTOMER_CUSTOMERL_LOAN foreign key (loanID)
      references loan (loanID) on delete cascade on update cascade;

alter table customerLoan add constraint FK_CUSTOMER_CUSTOMERL_CUSTOMER foreign key (customerID)
      references customer (customerID) on delete restrict on update restrict;

alter table department add constraint FK_DEPARTME_BANKDEPAR_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete cascade on update cascade;

alter table loan add constraint FK_LOAN_BANKLOAN_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete restrict on update cascade;

alter table payment add constraint FK_PAYMENT_PAY_LOAN foreign key (loanID)
      references loan (loanID) on delete restrict on update restrict;

alter table save add constraint FK_SAVE_SAVEBANK_BANKSUBB foreign key (bankName)
      references bankSubbranch (bankName) on delete restrict on update cascade;

alter table staff add constraint FK_STAFF_BELONG_DEPARTME foreign key (departmentNum)
      references department (departmentNum) on delete restrict on update cascade;

alter table saveCustomer add constraint FK_SAVECUST_SAVECUSTO_CUSTOMER foreign key (customerID)
      references customer (customerID) on delete restrict on update restrict;

alter table saveCustomer add constraint FK_SAVECUST_SAVECUSTO_SAVE foreign key (accountID)
      references save (accountID) on delete cascade on update cascade;

# 不能删除处于发放状态的贷款
DELIMITER //
create trigger loanDelete
Before delete on loan
for each row
begin
	declare a char;
	select state into a from loan where old.loanID=loan.loanID;
    if a = '1' then
		signal sqlstate 'HY000' set message_text = '贷款发放中';
	end if;
end //
DELIMITER ;

# 发放贷款金额不能超过贷款总额
# 发放贷款时更新loan表
DELIMITER //
create trigger loanState
After insert on payment
for each row
begin
	declare pay float8;
    declare total float8;
	select sum(cash) into pay from payment where payment.loanID=new.loanID;
    select loan.sum into total from loan where loan.loanID=new.loanID;
    if pay > 0 and pay < total then
		update loan set state='1' where loan.loanID=new.loanID;
        update loan set paySum=pay where loan.loanID=new.loanID;
	elseif pay=total then
		update loan set state='2' where loan.loanID=new.loanID;
        update loan set paySum=pay where loan.loanID=new.loanID;
	elseif pay>total then
		delete from payment where payment.loanID=new.loanID and payment.payTime=new.payTime and payment.cash=new.cash;
		signal sqlstate 'HY001' set message_text = '超出贷款金额';
	end if;
end //
DELIMITER //


#当账户进行存取款操作时更新账户余额
DELIMITER //
create trigger insertSaveInfo
after insert on saveInfo
for each row
begin
	update save set save.visitTime=new.visitTime where save.accountID=new.accountID;
    update save set save.balance=new.balance where save.accountID=new.accountID;
end //
DELIMITER //

DELIMITER //
create trigger insertChequeInfo
after insert on chequeInfo
for each row
begin
	update cheque set cheque.visitTime=new.visitTime where cheque.accountID=new.accountID;
    update cheque set cheque.balance=new.balance where cheque.accountID=new.accountID;
end //
DELIMITER //


insert into bankSubbranch values ('北京支行','北京',50000000);
insert into bankSubbranch values ('合肥支行','合肥',30000000);
insert into bankSubbranch values ('南昌支行','南昌',30000000);
insert into bankSubbranch values ('上海支行','上海',50000000);
insert into bankSubbranch values ('广州支行','广州',40000000);

insert into department values ('1001','360730199001011001','营销部',0,'北京支行');
insert into department values ('1002','360730199001011002','资产管理部',1,'北京支行');
insert into department values ('1003','360730199001011003','风险管理部',2,'北京支行');

insert into department values ('2001','360730199001012001','营销部',0,'上海支行');
insert into department values ('2002','360730199001012002','资产管理部',1,'上海支行');
insert into department values ('2003','360730199001012003','风险管理部',2,'上海支行');

insert into department values ('3001','360730199001013001','营销部',0,'广州支行');
insert into department values ('3002','360730199001013002','资产管理部',1,'广州支行');
insert into department values ('3003','360730199001013003','风险管理部',2,'广州支行');

insert into department values ('4001','360730199001014001','营销部',0,'南昌支行');
insert into department values ('4002','360730199001014002','资产管理部',1,'南昌支行');
insert into department values ('4003','360730199001014003','风险管理部',2,'南昌支行');

insert into department values ('5001','360730199001015001','营销部',0,'合肥支行');
insert into department values ('5002','360730199001015002','资产管理部',1,'合肥支行');
insert into department values ('5003','360730199001015003','风险管理部',2,'合肥支行');


insert into staff values ('360730199001011001','2018-1-1','北京','王杰','6869833','1001');
insert into staff values ('360730199001011002','2018-1-1','北京','刘杰','6869831','1001');
insert into staff values ('360730199001011003','2018-1-1','北京','周杰','6869843','1001');
insert into staff values ('360730199501011001','2018-1-1','北京','周涛','5869843','1001');
insert into staff values ('360730199501011002','2018-1-1','北京','周虎','7869843','1001');
insert into staff values ('360730199501011003','2018-1-1','北京','周艾','6839843','1001');
insert into staff values ('360730199501011004','2018-1-1','北京','周安','6809843','1001');

insert into staff values ('360730199001012001','2018-1-1','合肥','廖杰','6869853','2001');
insert into staff values ('360730199001012002','2018-1-1','合肥','姜杰','6866833','2002');
insert into staff values ('360730199001012003','2018-1-1','合肥','曾杰','6863833','2003');
insert into staff values ('360730199501012001','2018-1-1','合肥','廖浩','6869223','2001');
insert into staff values ('360730199501012002','2018-1-1','合肥','姜浩','6866113','2002');
insert into staff values ('360730199501012003','2018-1-1','合肥','曾浩','6863822','2003');


insert into staff values ('360730199001013001','2018-1-1','南昌','李杰','6269833','3001');
insert into staff values ('360730199001013002','2018-1-1','南昌','张杰','6769833','3002');
insert into staff values ('360730199001013003','2018-1-1','南昌','姚杰','6859833','3003');

insert into staff values ('360730199001014001','2018-1-1','上海','韩杰','6878833','4001');
insert into staff values ('360730199001014002','2018-1-1','上海','吴杰','6109822','4002');
insert into staff values ('360730199001014003','2018-1-1','上海','赵杰','6869093','4003');

insert into staff values ('360730199001015001','2018-1-1','广州','王杰','6869833','5001');
insert into staff values ('360730199001015002','2018-1-1','广州','王杰','6869833','5002');
insert into staff values ('360730199001015003','2018-1-1','广州','王杰','6869833','5003');
select * from bankSubbranch;
select * from department;
select * from staff;