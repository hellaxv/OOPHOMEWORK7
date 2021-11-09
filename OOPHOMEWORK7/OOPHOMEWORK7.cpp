#include <iostream>

void obzac()
{
	cout << "\n\n//====================================HomeWork====================================//\n\n";
}
void obzac1()
{
	cout << "\n\n//======================================END======================================//\n\n";
}
void obzac2()
{
	cout << "\n\n//-------------------------------------------------------------------------------//\n\n";
}

using namespace std;


/*HomeWork1
Создайте класс Date с полями день, месяц, год и методами доступа к этим полям. 
Перегрузите оператор вывода для данного класса. 
Создайте два "умных" указателя today и date. 
Первому присвойте значение сегодняшней даты. 
Для него вызовите по отдельности методы доступа к полям класса Date, 
а также выведите на экран данные всего объекта с помощью перегруженного оператора вывода. 
Затем переместите ресурс, которым владеет указатель today в указатель date. 
Проверьте, являются ли нулевыми указатели today и date и выведите соответствующую информацию об этом в консоль.
*/
class Date
{
private:
	int day, month, year;
public:
	Date() : day(0), month(0), year(0) {}
	Date(int inputDay, int inputMonth, int inputYear) : day(inputDay), month(inputMonth), year(inputYear) {}

	int getDay()
	{
		return day;
	}

	int getMonth()
	{
		return month;
	}

	int getYear()
	{
		return year;
	}

	friend ostream& operator<<(ostream& out, const Date& date);
};

ostream& operator<<(ostream& out, const Date& date)
{
	out << "\n" << date.day << "/" << date.month << "/" << date.year;
	return out;
}

/*HomeWork2
По условию предыдущей задачи создайте два умных указателя date1 и date2. 
Создайте функцию, которая принимает в качестве параметра два умных указателя типа Date и сравнивает их между собой (сравнение происходит по датам). 
Функция должна вернуть более позднюю дату.
Создайте функцию, которая обменивает ресурсами (датами) два умных указателя, переданных в функцию в качестве параметров.
*/
Date findLaterDate(const unique_ptr<Date>& date1, const unique_ptr<Date>& date2)
{
	if (date1->getYear() > date2->getYear()) return *date1;
	else if (date1->getYear() < date2->getYear()) return *date2;

	if (date1->getMonth() > date2->getMonth()) return *date1;
	else if (date1->getMonth() < date2->getMonth()) return *date2;

	if (date1->getDay() > date2->getDay()) return *date1;
	else if (date1->getDay() < date2->getDay()) return *date2;

	return *date1;
}

void swapDates(unique_ptr<Date>& date1, unique_ptr<Date>& date2)
{
	swap(date1, date2);
}


int main()
{
	{
		//HomeWork1
		unique_ptr<Date> today(new Date(10, 11, 2021));
		cout << "\n" << today->getDay() << ".0" << today->getMonth() << "." << today->getYear();
		cout << *today;
		unique_ptr<Date> date(new Date());
		swap(today, date);
		cout << *date;
		today = nullptr;
		if (date == nullptr) cout << "\nPointer \"date\" is null";
		else cout << "\nPointer \"date\" is not null";
		if (today == nullptr) cout << "\nPointer \"today\" is null";
		else cout << "\nPointer \"today\" is not null";
	}

	{
		//HomeWork2
		unique_ptr<Date> date1(new Date(17, 3, 19));
		unique_ptr<Date> date2(new Date(16, 3, 19));
		cout << "\n\nLater date is: " << findLaterDate(date1, date2);
		cout << "\n\nOld dates:" << *date1;
		cout << *date2;
		swapDates(date1, date2);
		cout << "\nNew dates (swapped):" << *date1;
		cout << *date2;
	}
}
