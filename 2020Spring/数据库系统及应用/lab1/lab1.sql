use test;
drop table if exists Borrow;
drop table if exists Book;
drop table if exists Reader;
#drop table Borrow;
#drop table Book;
#drop table Reader;

#1:创建表，插入测试数据
create table Book (
	ID char(8) primary key,
    name varchar(8) not null,
    author varchar(10),
    price float,
    status int default 0 check(status=0 or status=1)
	);

create table Reader (
	ID char(8) primary key,
    name varchar(10),
    age int,
    address varchar(20)
    );
    
create table Borrow (
	Book_ID char(8),
    Reader_ID char(8),
    Borrow_Date date,
    Return_Date date,
    foreign key(book_ID) references Book(ID) On Delete Cascade,
    foreign key(Reader_ID) references Reader(ID) On Delete Cascade,
    Constraint PK_Borrow Primary Key(Book_Id,Reader_ID)
    );
    
insert into Book values ('B_ID0001','Book1','author1',32.8,1);
insert into Book values ('B_ID0002','Book2','author1',22.8,1);
insert into Book values ('B_ID0003','Book3','author1',328,0);
insert into Book values ('B_ID0004','Book4','author2',38.8,1);
insert into Book values ('B_ID0005','Book5','author2',20,1);
insert into Book values ('B_ID0006','Book6','author2',42,0);
insert into Book values ('B_ID0007','Book7','author3',11,0);
insert into Book values ('B_ID0008','Book8','author3',53,0);
insert into Book values ('B_ID0009','Book9','author3',42,0);
insert into Book values ('B_ID0010','Book10','author4',3,1);
insert into Book values ('B_ID0011','Book11','author4',10.4,1);
insert into Book values ('B_ID0012','Book12','author4',15,1);
insert into Book values ('B_ID0013','Book13','author5',19,1);
insert into Book values ('B_ID0014','Book14','author6',10,0);
insert into Book values ('B_ID0015','Oracle15','author7',100,0);
insert into Book values ('B_ID0016','Oracle16','author7',100,0);
insert into Book values ('B_ID0017','Oracle17','author7',100,0);
insert into Book values ('B_ID0018','Oracle18','author7',100,0);
insert into Book values ('B_ID0019','19Oracle','author7',100,0);
insert into Book values ('B_ID0020','2Oracle1','author7',100,0);
insert into Book values ('B_ID0025','Book25','Ullman',100,0);
insert into Book values ('B_ID0026','Book26','Ullman',200,0);
insert into Book values ('B_ID0027','Book27','Ullman',300,0);

insert into Reader values ('R_ID0001','Reader1',18,'ustc-2-215');
insert into Reader values ('R_ID0002','Reader2',17,'ustc-2-216');
insert into Reader values ('R_ID0003','Reader3',19,'ustc-2-217');
insert into Reader values ('R_ID0004','Reader4',18,'ustc-2-218');
insert into Reader values ('R_ID0005','Reader5',14,'ustc-3-215');
insert into Reader values ('R_ID0006','Reader6',22,'ustc-3-216');
insert into Reader values ('R_ID0007','Reader7',24,'ustc-3-217');
insert into Reader values ('R_ID0008','Reader8',15,'ustc-3-218');
insert into Reader values ('R_ID0009','Reader9',18,'ustc-4-215');
insert into Reader values ('R_ID0010','Reader10',18,'ustc-4-216');
insert into Reader values ('R_ID0011','Reader11',25,'ustc-4-217');
insert into Reader values ('R_ID0012','Reader12',28,'ustc-4-218');
insert into Reader values ('R_ID0013','Reader13',18,'ustc-4-219');
insert into Reader values ('R_ID0014','Reader14',18,'ustc-5-219');
insert into Reader values ('R_ID0015','Reader15',18,'ustc-5-219');
insert into Reader values ('R_ID0016','Rose',18,'ustc-5-220');
insert into Reader values ('R_ID0017','李林',18,'ustc-5-221');

insert into Borrow values ('B_ID0001','R_ID0001','2020-03-10',NULL);
insert into Borrow values ('B_ID0002','R_ID0002','2020-03-20',NULL);
insert into Borrow values ('B_ID0004','R_ID0004','2020-01-10',NULL);
insert into Borrow values ('B_ID0005','R_ID0005','2020-02-11',NULL);
insert into Borrow values ('B_ID0001','R_ID0006','2000-02-12','2000-03-09');
insert into Borrow values ('B_ID0001','R_ID0016','2020-02-12','2020-03-09');
insert into Borrow values ('B_ID0002','R_ID0016','2020-02-22','2020-03-01');
insert into Borrow values ('B_ID0003','R_ID0016','2020-02-12','2020-03-10');
insert into Borrow values ('B_ID0012','R_ID0016','2020-03-12',NULL);
insert into Borrow values ('B_ID0013','R_ID0016','2020-02-12',NULL);
insert into Borrow values ('B_ID0005','R_ID0017','2018-02-12','2018-3-14');
insert into Borrow values ('B_ID0001','R_ID0017','2020-01-12','2020-1-14');
insert into Borrow values ('B_ID0010','R_ID0017','2020-02-08',NULL);
insert into Borrow values ('B_ID0011','R_ID0017','2020-02-12',NULL);

select * from Book;
select * from Reader;
select * from Borrow;

#2:实体完整性，参照完整性，用户自定义完整性
#2.1：主键不能为空或者重复
#insert into Book (name,author,price,status) values ('Book20','author10',23.9,0);
#insert into Borrow (Book_ID,Borrow_Date,Return_Date) values ('B_ID0019','2020-03-13','2020-05-02');
#insert into Borrow (Borrow_Date,Return_Date) values ('2020-03-13','2020-05-02');
#insert into Reader values ('R_ID0004','Reader5',14,'ustc-10-218');
#2.2:任一个外码值必须等于被参照关系 S 中所参照的候选码的某个值
#insert into Borrow values ('B_ID1111','R_ID0001','2020-03-10','2020-05-19');
#insert into Borrow values ('B_ID0001','R_ID1111','2020-03-10','2020-05-19');
#2.3:用户自定义完整性，Book_name不为空，status为0或1
#insert into Book (ID,author,price,status) values ('B_ID0200','author1',12,0);
#insert into Book values ('B_ID0025','Book25','author7',100,10);
#insert into Book values ('B_ID0035','Book35','author7',100,11);

#3
#3.(1)
select ID,address
from Reader
where name='Rose';
#3.(2)
select Book.name,Borrow.Borrow_date
from Book,Borrow,Reader
where Reader.name='Rose' and Reader.ID=Borrow.Reader_ID and Borrow.Book_ID=Book.ID ;
#3.(3)
select Reader.name
from Reader
where Reader.ID not in (select distinct Reader.ID
						from Borrow,Reader
                        where Borrow.Reader_ID=Reader.ID
                        ) ;
#3.(4)
select Book.name,Book.price
from Book
where Book.author = 'Ullman';
#3.(5)
select Book.ID,Book.name
from Reader,Book,borrow
where Reader.name='李林' and Reader.ID=Borrow.Reader_ID and Borrow.Book_ID=Book.ID and Borrow.Return_Date is NULL;
#3.(6)
select Reader.name
from Reader,Borrow
where Reader.ID=Borrow.Reader_ID
group by Borrow.Reader_ID
having count(*)>3;
#3.(7):在读者中减去借过李林借过的书的名单
select Reader.name,Reader.ID
from Reader
where Reader.ID not in ( select Reader.ID
						 from Reader,Borrow
                         where Reader.ID = Borrow.Reader_ID and Borrow.Book_ID in ( select Borrow.Book_ID
																					from Reader,Borrow
                                                                                    where Reader.ID=Borrow.Reader_ID and Reader.name='李林')
                                                                                   ) ;
#3.(8)
select Book.name,Book.ID
from Book
where instr(Book.name,'Oracle')>0;
#3.(9)
drop view Reader_view;
create view Reader_view (Reader_ID,Reader_name,Book_ID,Book_name,Borrow_date)
as select Reader.ID,Reader.name,Book.ID,Book.name,Borrow.Borrow_Date
	from Reader,Book,borrow
	where Reader.ID = Borrow.Reader_ID and Book.ID=Borrow.Book_ID;
select *
from Reader_view;
select Reader_view.Reader_ID,count(Reader_view.Book_ID) as Book_Number
from Reader_view
where Reader_view.Borrow_date between date_sub(curdate(),interval 1 year) and curdate()
group by Reader_view.Reader_ID;

#4
#insert into Book values ('B_ID1115','Book5','author2',20,1);
#select * from Book;
drop procedure if exists update_BookID;
delimiter //
create procedure update_BookID(In Book_ID_Before char(8),In BooK_ID_After char(8))
begin
insert into Book(ID,name,author,price)
select Book_ID_After,Book.name,Book.author,Book.price
from Book
where Book.ID=Book_ID_Before;

update Borrow
set Book_ID=Book_ID_After
where Book_ID=Book_ID_before;

delete from Book
where Book.ID=Book_ID_Before;
end //
delimiter ;
select * from Book;
select * from Borrow;
call update_BookID('B_ID0001','A_ID0001');
select * from Book;
select * from Borrow;

#5
#对每本书，如果它的status是0，说明其在馆，检查Borrow中，该书的还书期应该都不为空，否则错误数加1
#如果它的status是1，说明被借出，检查Borrow，应该存在一项该书的还书期为空，否则错误加1
drop procedure if exists check_status;
delimiter //
create  procedure check_status(out wrong_number int)
begin
Declare state INT default 0;
Declare num INT default 0;
declare cs_BookID char(8);
declare cs_BookName varchar(10);
declare cs_BookPrice float;
declare cs_BookAuthor varchar(10);
declare cs_BookStatus int;
DECLARE cs_Book CURSOR for select * from Book ;
Declare continue Handler for NOT FOUND set state=1;
set wrong_number=0;
open cs_Book;

Repeat
	Fetch cs_Book Into cs_BookID,cs_BookName,cs_BookAuthor,cs_BookPrice,cs_BookStatus;
	if state = 0 then
		if cs_BookStatus=0 then 
			select count(*)
            from Borrow
            where Borrow.Book_ID=cs_BookID and Borrow.Return_Date is null
            into num;
            if(num>0) then
				set wrong_number=wrong_number+1;
			end if;
		elseif cs_BookStatus=1 then
			select count(*)
            from Borrow
            where Borrow.Book_ID=cs_BookID and Borrow.Return_Date is null
            into num;
            if(num=0) then
				set wrong_number=wrong_number+1;
			end if;
		end if;
    end if;
Until state=1
End Repeat;

Close cs_Book ;
end //
delimiter ;
select * from Book;
select * from Borrow;
call check_status(@wrong_Num);
select @wrong_Num;
update Book
set Book.Status=1
where Book.ID='A_ID0001';
call check_status(@wrong_Num);
select @wrong_Num;

#6
drop trigger if exists Borrow_A_Book;
delimiter //
create trigger Borrow_A_Book 
after insert 
on Borrow
for each row
Begin
	Declare num INT default 0;
	select count(*)
	from Borrow
	where Borrow.Book_ID=new.Book_ID and Borrow.Return_Date is null
	into num;
    if(num>0) then
		update Book
        set Book.status=1
        where Book.ID=new.Book_ID;
	elseif(num=0) then
		update Book
        set Book.status=0
        where Book.ID=new.Book_ID;
	end if;
end //
delimiter ;
drop trigger if exists Return_A_Book;
delimiter //
create trigger Return_A_Book 
after update
on Borrow
for each row
Begin
	Declare num INT default 0;
	select count(*)
	from Borrow
	where Borrow.Book_ID=new.Book_ID and Borrow.Return_Date is null
	into num;
    if(num>0) then
		update Book
        set Book.status=1
        where Book.ID=new.Book_ID;
	elseif(num=0) then
		update Book
        set Book.status=0
        where Book.ID=new.Book_ID;
	end if;
end //
delimiter ;
select * from Book;
select * from Borrow;
insert into Borrow values ('B_ID0015','R_ID0005','2020-02-11',NULL);
insert into Borrow values ('B_ID0016','R_ID0005','2020-02-11',NULL);
insert into Borrow values ('B_ID0017','R_ID0005','2020-02-11',NULL);
insert into Borrow values ('B_ID0018','R_ID0006','2000-02-12','2000-03-09');
update Borrow set Return_Date='2020-03-31' where Book_ID='A_ID0001' and Reader_ID='R_ID0001';
update Borrow set Return_Date='2020-03-31' where Book_ID='B_ID0002' and Reader_ID='R_ID0002';
select * from Book;
select * from Borrow;
