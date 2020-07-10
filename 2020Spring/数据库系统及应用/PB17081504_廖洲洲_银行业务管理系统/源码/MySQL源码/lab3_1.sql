
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