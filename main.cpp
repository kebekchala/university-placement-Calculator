#include <iostream>
#include <fstream>
#include <windows.h>
#include <limits>
#include <conio.h>
#include <iomanip>
using namespace std;
// object variables
struct student{
    int id,
    score,
    university_order[100],
    department_order[100],
    disabled;
    char name[50];
    char sex;};
struct department{
    char name[30];
    int code;
    int capacity;};
struct university{
    char name[30];
    int code;
    int departmentCount;
    department departments[100];
    };
struct Allocation{
    student Student;
    university University;
    department Department;
};
enum sorttypes{
    by_id,by_name,by_score
};
// create a space on the heap for data interpretation
    university* universities=new university[100];
    student* students=new student[10000];
    department* departments=new department[100];
    Allocation* result=new Allocation[10000];
void // functions
Manager_Account(void),
Student_Account(void),
ALLOCATE_STUDENTS(void),
SortStudents(student [],int,int),
Animate2(string,unsigned int,int space=0),
ErrorDialog(string message="Invalid Character",int=1000,int = 40),
SuccessDialog(string,int=1000,int=40);
inline bool notgood(),
notgood(int*,int =1,int =100);
department DepMatch(int),
DepMatch(department[],int,int);
bool verfypassword(string,string);
// global variables are below
    char// strings
    headname[20];
    string headpassword,globalspace="                                        ",usernamed;
    int // integers
    AllocatedCount=0, // the number of students allocated
    global1=-1, // an integer for simple temporary purposes
    depchoiceCount=0, // how many department choices the student chose
    unichoiceCount=0, // how many university choices the student chose
    studentCount=0,  // the number of students
    universityCount=0, // the number of universities
    departmentCount=0  // the number of departments
    ;
    float femaleSupport=2.0; // in how much percentage their score is increased for allocation
    char // characters
    globalchar  // used for entering sex
    ;
    bool //flags
    allocated=false, // to see if allocation has been done
    filetaken=false, // to see if files have been stored to the stream
    okey=true;
int main()
{
    int LoginAttempt=0;
    // Transferring user name and password to the stream
    fstream user("user.txt",ios::in);
    if(!user.is_open())
    {
        cout<<"user.txt file is missing. program terminated";
        return 0;
    }
    int userCount=0;

    struct users{
        char username[20];
        string password;
    }Users[20];

    for(int i=0;;i++)
    {
        user.getline(Users[i].username,20,'>');
        getline(user,Users[i].password,'\n');
        userCount++;
        if(user.peek()==EOF)break;
    }
    user.close();
    /*****************************MAIN PAGE******************************/
    system("color f0");
    Animate2("WELCOME TO AE STUDENT PLACEMENT APPLICATION",1,20);
    Sleep(5);
    cout<<endl;
    AccountChoose:
    system("cls");
    cout<<globalspace<<"1.Log in as Manager"<<endl;
    cout<<globalspace<<"2.Log in as Student"<<endl;
    cout<<globalspace<<"3.About"<<endl;
    cout<<globalspace<<"4.Exit"<<endl;
    cout<<globalspace<<" __"<<endl<<globalspace<<"|";
    int local1;
    cin>>local1;
    if(notgood(&local1,1,4)){
        ErrorDialog("Invalid Character. Enter a Valid Number",1000,40);
        goto AccountChoose;}
    switch(local1)
    {
    case 1:
        cin.ignore(1);
        // manager account goes here
        Login1:
        global1=-1;
        system("cls");
        cout<<globalspace<<"Enter user name:"<<endl<<globalspace<<" ________________"<<endl<<globalspace<<"|";
        cin.getline(headname,21,'\n');
        for(int i=0;i<userCount;i++)
        {
            if(!strcmp(headname,Users[i].username)){
                global1=i;
                usernamed=Users[i].username;
                break;}
        }
        if(global1==-1) // if username is not found
        {
            ErrorDialog("Invalid User name!!!\nretry",1000,40);
            goto Login1;
        }
        Login2:
        if(LoginAttempt<3){
            system("cls");
        cout<<globalspace<<"Enter password:"<<endl
        <<globalspace<<" _______________"<<endl
        <<globalspace<<"|";
        char a=getch();
        while(a!=13)
        {
            headpassword.push_back(a);
            cout<<'*';
            a=getch();
        }
        if(verfypassword(Users[global1].password,headpassword))
            Manager_Account();
        else {
                headpassword="";
                ErrorDialog("Incorrect password!!!",1000,40);
                LoginAttempt++;
                goto Login2;}
        } else LoginAttempt=0;
        system("cls");
        goto AccountChoose;
        break;
    case 2:
        // students go here
        //goto student function
        Student_Account();
        system("cls");
        goto AccountChoose;
        break;
    case 3: //about
        system("cls");
        cout<<globalspace<<"A student placement program developed by;"<<endl;
        cout<<globalspace<<"1. Kirubel Adamu"<<endl
        <<globalspace<<"2. Nahom Balcha"<<endl
        <<globalspace<<"3. Meseret Kifle"<<endl
        <<globalspace<<"4. Kebek Chala"<<endl
        <<globalspace<<"5. Natnael Mesfin"<<endl
        <<globalspace<<"6. Kidist Worku"<<endl
        <<globalspace<<"7. Natnael Tesfaye"<<endl;
        getch();
        goto AccountChoose;
        break;
    case 4:
        break;
    default:
        system("cls");
        goto AccountChoose;
    }
    return 0;
}
// function implementation
void Manager_Account(void)
{
    int* errlist=new int[departmentCount];

    int* errlist2=new int[universityCount];
    int errcount=0;
    bool error=false;
    Managers:
    fstream campus,learner,subject,report;
    if(!filetaken){
    // check the existence of the files
        campus.open("Universities.txt",ios::in);
        if(!campus.is_open())
            {ErrorDialog("universities file is missing",2000,40);
            okey=false;}
        learner.open("Students.txt",ios::in);
        if(!learner.is_open())
            {ErrorDialog("students file is missing",2000,40);
            okey=false;}
        subject.open("Departments.txt",ios::in);
        if(!subject.is_open())
        {
            ErrorDialog("departments file is missing",2000,40);
            okey=false;
        }
        // Load the files on the heap
        if(okey) //if there is no error in opening the files
        {
            // entering university data
            char test;
            campus.ignore(80,'\n');
            for(int i=0;campus;i++)
            {
                universities[i].departmentCount=0;
                campus>>universities[i].code;
                campus.ignore(1);
                campus.getline(universities[i].name,80,'_');
                for(int j=0;;j++){
                    campus>>universities[i].departments[j].code;
                    campus.ignore(1);
                    campus>>universities[i].departments[j].capacity;
                    campus>>test;
                    universities[i].departmentCount++;
                    if(test==';') break;
                                }
                universityCount++;
                if(campus.peek()==EOF) break;
                else campus.ignore(1);
            }
            SuccessDialog("Universities file loaded successfully!");
            campus.close();
            // entering student file
            learner.ignore(200,'\n');
            for(int i=0;!learner.eof();i++)
            {
                learner.getline(students[i].name,50,'_');
                learner.get(students[i].sex);
                learner.ignore(1);
                learner>>students[i].disabled;
                learner.ignore(1);
                learner>>students[i].id;
                learner.ignore(1);
                learner>>students[i].score;
                learner.ignore(1);
                for(int j=0;;j++)
                {
                    learner>>students[i].department_order[j];
                    learner.get(test);
                    if(i==0)depchoiceCount++;
                    if(test=='_') break;
                }
                for(int j=0;;j++)
                {
                    learner>>students[i].university_order[j];
                    learner>>test;
                    if(i==0)unichoiceCount++;
                    if(test==';') break;
                }

                studentCount++;
                if(learner.peek()==EOF) break;
                else learner.ignore(1);
            }
            SuccessDialog("students file loaded successfully!");
            learner.close();
            // entering departments
            subject.ignore(100,'\n');
            for(int i=0;!subject.eof();i++)
            {
                subject>>departments[i].code;
                subject.get(test);
                subject.getline(departments[i].name,20,'\n');
                departmentCount++;
            }
            SuccessDialog("departments file loaded successfully!");
            subject.close();
        }
        filetaken=true;
    }
    // management goes here
    ManagersLoc:
    system("cls");
    if(okey){ // if there is no error on opening the files
    cout<<globalspace<<"Welcome "<<usernamed<<endl
    <<globalspace<<"what do you want to do?\n"
    <<globalspace<<"1.see universities\n"
    <<globalspace<<"2.see all departments\n"
    <<globalspace<<"3.see students list\n"
    <<globalspace<<"4.edit universities\n"
    <<globalspace<<"5.add university\n"
    <<globalspace<<"6.delete university\n"
    <<globalspace<<"7.add students\n"
    <<globalspace<<"8.edit student\n"
    <<globalspace<<"9.add department\n"
    <<globalspace<<"10.do an allocation\n";
    if(allocated)
        cout<<globalspace<<"11.see allocation result\n"
        <<globalspace<<"12.prepare report\n";
    else cout<<globalspace<<"// choice 11 and 12 will be visible when you do an allocation\n";
    cout<<globalspace<<"13.Log Out\n";
    int local2;
    int min=departmentCount,max=0;
    char string1[30];
    cout<<globalspace<<" __"<<endl<<globalspace<<"|";
    cin>>local2;
    if(notgood(&local2,1,13)){
        ErrorDialog();
        goto ManagersLoc;}
    system("cls");
    switch(local2)
    {
    case 1:// list universities and their properties
        LOCQ:
        system("cls");
        cout.setf(ios::left);
        for(int i=0;i<universityCount;i++)
        {
            cout<<endl<<globalspace<<"      _"<<universities[i].name<<'_'<<endl;
            cout<<globalspace<<setw(20)<<"[Departments]"<<setw(15)<<"[Capacity]"<<endl;
            for(int j=0;j<universities[i].departmentCount;j++)
                cout<<globalspace<<setw(20)<<departments[universities[i].departments[j].code].name<<setw(15)<<universities[i].departments[j].capacity<<endl;
        }
        cout<<globalspace<<"Enter ESC to continue...";
        if(getch()==27)
        goto Managers;
        else goto LOCQ;
        break;
    case 2: // list available departments across the agency
        LD:
        system("cls");
        for(int i=0;i<departmentCount;i++)
        {
            cout<<globalspace<<i+1<<". "<<departments[i].name<<endl;
        }
        cout<<globalspace<<"press ESC to continue...";
        if(getch()==27)
        goto Managers;
        goto LD;
        break;
    case 3: //list students with their identity data and their department and university choices
        LOc:
        for(int i=0;i<studentCount;i++)
        {
            cout.setf(ios::left);
            cout<<endl<<globalspace<<"         "<<students[i].name<<endl;
            cout<<globalspace<<"ID:        "<<students[i].id<<endl;
            cout<<globalspace<<"sex:       ";if(students[i].sex=='F')cout<<"Female"<<endl;else cout<<"Male"<<endl;
            cout<<globalspace<<"score:     "<<students[i].score<<endl;
            cout<<globalspace<<"condition: ";if(students[i].disabled==1)cout<<"Disabled"<<endl;else cout<<"Normal"<<endl;
            cout<<globalspace<<"department choices:"<<endl;
            cout.setf(ios::right);
            for(int j=0;j<departmentCount;j++)
            {
                cout<<globalspace<<setw(5)<<j+1<<setw(1)<<"."<<setw(20)<<departments[students[i].department_order[j]].name<<endl;
            }
            cout.setf(ios::left);
            cout<<globalspace<<"university choices:"<<endl;
            cout.setf(ios::right);
            for(int j=0;j<universityCount;j++)
            {
                cout<<globalspace<<setw(5)<<j+1<<setw(1)<<"."<<setw(20)<<universities[students[i].university_order[j]].name<<endl;
            }
        }
        cout<<globalspace<<"Enter ESC to continue...";
            if(getch()==27)
        goto Managers;
        else goto LOc;
        break;
    case 4: //edit universities
        Managers4:
        cout<<globalspace<<"Select the university you want to edit\n";
        int universitynum;
        // list the universities for the user to choose one
        for(int i=0;i<universityCount;i++)
        {
            cout<<globalspace<<i+1<<". "<<universities[i].name<<endl;
        }
        cout<<globalspace<<" __"<<endl<<globalspace<<"|";
        cin>>universitynum;
        if(notgood(&universitynum,1,universityCount))
        {
            ErrorDialog();
            goto Managers4;
        }
        universitynum--;
        Managers3:
        int local21;
        system("cls");
        cout<<globalspace<<"1.change name\n"
        <<globalspace<<"2.delete department\n"
        <<globalspace<<"3.Add department\n"
        <<globalspace<<"4.edit capacity of departments\n"
        <<globalspace<<"5.go back\n";
        cout<<globalspace<<" __"<<endl<<globalspace<<"|";
        cin>>local21;
        if(notgood(&local21,1,6)){
            ErrorDialog();
            goto Managers3;
        }
        system("cls");
        char named[30];
        switch(local21)
        {
        case 1://simple changing of name
            cin.ignore(1);
            cout<<globalspace<<"Enter name\n";
            cout<<globalspace<<" __________________"<<endl<<globalspace<<"|";
            cin.getline(named,30,'\n');
            strcpy(universities[universitynum].name,named);
            SuccessDialog("university renamed!!!");
            goto Managers3;
            break;
        case 2://delete department
            Managers5:
            if(universities[universitynum].departmentCount>0){
            cout<<globalspace<<"Select the department to be deleted\n";
            // list the departments here
            for(int i=0;i<universities[universitynum].departmentCount;i++)
                cout<<globalspace<<i+1<<". "<<DepMatch(universities[universitynum].departments[i].code).name<<endl;
            cout<<globalspace<<" __"<<endl<<globalspace<<"|";
            cin>>local21;
            if(notgood(&local21,1,universities[universitynum].departmentCount))
            {
                ErrorDialog();
                goto Managers5;
            }
            //delete the department at local21;
            cout<<endl<<globalspace<<"Delete this Department???\n"<<globalspace<<"Click Enter to confirm, any other key otherwise...";
            if( getch()==13)// if the user clicks ENTER
            {
                for(int i=local21;i<departmentCount;i++)
                    universities[universitynum].departments[i-1]=universities[universitynum].departments[i];
                universities[universitynum].departmentCount--;
                SuccessDialog("department deleted!!!");
            } else ErrorDialog("Operation Canceled!!!");
            goto Managers3;
            }
        else {
                ErrorDialog("There are no departments in this university!!!");
                goto Managers3;}
            break;
        case 3: //add available departments
            Managers7:
            if(universities[universitynum].departmentCount==departmentCount)
            {
                ErrorDialog("This University has all available departments!!!");
                goto Managers3;
            }
            cout<<globalspace<<"These are available departments that you can add to this university.\n"<<globalspace<<"select one...\n";
            // list all departments that are not in this university
            min=departmentCount,max=0,errcount=0;
            error=false;
            for(int i=0;i<departmentCount;i++){
            if(!strcmp(DepMatch(universities[universitynum].departments,universities[universitynum].departmentCount,departments[i].code).name,"NULL"))
            {cout<<globalspace<<i+1<<". "<<departments[i].name<<endl;
            if(i<min)min=i;
            if(i>max)max=i;
            } else {
                errlist[errcount]=i+1;
                errcount++;
            }
            }
            cout<<globalspace<<" __"<<endl<<globalspace<<"|";
            cin>>local21;
            for(int i=0;i<errcount;i++)
            if(local21==errlist[i]) {error=true; break;}
            if(notgood(&local21,min+1,max+1) || error) {
                    ErrorDialog();goto Managers7;}
            universities[universitynum].departments[universities[universitynum].departmentCount].code=departments[local21-1].code;
            cout<<globalspace<<"Enter capacity of the department\n";
            cout<<globalspace<<" __"<<endl<<globalspace<<"|";
            cin>>local2;
            universities[universitynum].departments[universities[universitynum].departmentCount].capacity=local2;
            universities[universitynum].departmentCount++;
            SuccessDialog("department added");
            goto Managers3;
            break;
        case 4://edit capacity of departments
            Managers6:
            cout<<globalspace<<"select the department to edit\n";
            //list the departments
            for(int i=0;i<universities[universitynum].departmentCount;i++)
                cout<<globalspace<<i+1<<". "<<DepMatch(universities[universitynum].departments[i].code).name<<endl;
            cout<<globalspace<<" __"<<endl<<globalspace<<"|";
            cin>>local21;
            if(notgood(&local21,1,universities[universitynum].departmentCount))
            {
                ErrorDialog();
                goto Managers6;
            }
            local1:
            system("cls");
            cout<<globalspace<<"The capacity of this department is "<<universities[universitynum].departments[local21-1].capacity<<endl;
            cout<<globalspace<<"Enter the new capacity of the department\n";
            cout<<globalspace<<" __"<<endl<<globalspace<<"|";
            cin>>local2;
            if(notgood()){ErrorDialog();goto local1;}
            universities[universitynum].departments[local21-1].capacity=local2;
            SuccessDialog("Department Edited!!!");
            goto Managers3;
            break;
        case 5://return to main page
            goto Managers;
            break;
        default:
            goto Managers3;
        }
        break;
    case 5: //add universities
        cin.clear();
        cin.ignore(20,'\n');
            cout<<globalspace<<"Enter name for the university\n";
            cout<<globalspace<<" _________________"<<endl<<globalspace<<"|";
            cin.getline(named,30,'\n');
            strcpy(universities[universityCount].name,named);
            universities[universityCount].code=universityCount;
            universityCount++;
            universities[universityCount-1].departmentCount=0;
            Managers81:
            system("cls");
            if(universities[universityCount-1].departmentCount==departmentCount)
            {
                ErrorDialog("All departments are added!!!");
                goto Managers;
            }
            cout<<globalspace<<"add departments\n";
            //list departments for the user
            min=departmentCount,max=0;
            errcount=0;
            error=false;
            for(int i=0;i<departmentCount;i++){
                bool isinvalid=false;
                for(int j=0;j<universities[universityCount-1].departmentCount;j++){
                    if(universities[universityCount-1].departments[j].code==departments[i].code){
                        isinvalid=true;
                        }
                   }
                if(!isinvalid){
                    cout<<globalspace<<i+1<<". "<<departments[i].name<<endl;
                    if(i<min)min=i;
                    if(i>max)max=i;
                   } else {
                   errlist[errcount]=i+1;
                        errcount++;}
            }
            cout<<globalspace<<" __"<<endl<<globalspace<<"|";
            cin>>local21;
            for(int i=0;i<errcount;i++)
            if(local21==errlist[i]) {error=true; break;}
            if(notgood(&local21,min+1,max+1) || error) {
                    ErrorDialog();goto Managers81;}
            cout<<globalspace<<"Enter the capacity of this department\n";
            cout<<globalspace<<" __"<<endl<<globalspace<<"|";
            cin>>local2;
            if(notgood()){ErrorDialog();goto Managers81;}
            universities[universityCount-1].departments[universities[universityCount-1].departmentCount].code=local21-1;
            universities[universityCount-1].departments[universities[universityCount-1].departmentCount].capacity=local2;
            universities[universityCount-1].departmentCount++;
            cout<<globalspace<<"Click Enter to add more...any other key otherwise";
            if((int)getch()==13) goto Managers81;
            goto Managers;
        break;
    case 6://delete university
        Managers9:
        cout<<globalspace<<"Select the university you want to delete\n";
        // list the universities
        for(int i=0;i<universityCount;i++)
            cout<<globalspace<<i+1<<". "<<universities[i].name<<endl;
        cout<<globalspace<<" __"<<endl<<globalspace<<"|";
        cin>>local21;
        if(notgood(&local21,1,universityCount))
        {ErrorDialog();goto Managers9;}
        cout<<globalspace<<"Click Enter to confirm...any other key otherwise...";
        if((int)getch()==13) //delete the university
        {
            for(int i=local21;i<universityCount;i++)
            {
                universities[i-1]=universities[i];
            }
            universityCount--;
            SuccessDialog("University Deleted!!!");
        }else {ErrorDialog("Operation canceled!");goto Managers;}
        goto Managers;
        break;
    case 7: //add students
        cin.clear();
        cin.ignore(10,'\n');
        Managers10:
        cout<<globalspace<<"Enter full name of the student. (first_name father_name grandfather_name)\n";
        cout<<globalspace<<" __________________"<<endl<<globalspace<<"|";
        cin.getline(string1,30,'\n');
        min=0;
        for(int i=0;i<30;i++)
            if(string1[i]==(char)32)min++;
        if(min!=2){ErrorDialog("Invalid Name!!!");goto Managers10;}
        strcpy(students[studentCount].name,string1);
        Managers101:
        cout<<globalspace<<"enter sex of the student.\n";
        cout<<globalspace<<" __"<<endl<<globalspace<<"|";
        cin>>globalchar;
        globalchar=toupper(globalchar);
        if(globalchar!='F'&&globalchar!='M'){ErrorDialog("Invalid Sex Value!!!");goto Managers101;}
        students[studentCount].sex=globalchar;
        ID:
        cout<<globalspace<<"enter id for the student.\n";
        cout<<globalspace<<" _______"<<endl<<globalspace<<"|";
        cin>>local21;
        if(notgood()){ErrorDialog();goto ID;}
        if(local21<100000||local21>999999)
        {
            ErrorDialog("ID must be b/n 100000 and 999999");goto ID;
        }
        for(int i=0;i<studentCount;i++)
            if(students[i].id==local21){ErrorDialog("This ID is Taken. Enter a correct ID");goto ID;}
        students[studentCount].id=local21;
        Score:
        cout<<globalspace<<"enter the score of the student.\n";
        cout<<globalspace<<" __"<<endl<<globalspace<<"|";
        cin>>local21;
        if(notgood()){ErrorDialog();goto Score;}
        if(local21<352||local21>700){ErrorDialog("Score must be b/n 352 and 700",2000);goto Score;}
        students[studentCount].score=local21;
        cout<<globalspace<<"is the student disabled in some way? if so click y , any other key otherwise...\n";
        if(getch()=='y') students[studentCount].disabled=1;
        else students[studentCount].disabled=0;
        // list universities for the user
        for(int k=0;k<universityCount;k++){
            system("cls");
            looploc:
            errcount=0;
            error=false;
            cout<<globalspace<<"Editing university choice\n";
            for(int i=0;i<universityCount;i++){
            bool isinvalid=false;
            for(int j=0;j<k;j++)
                if(universities[i].code==students[studentCount].university_order[j])
                    {isinvalid=true;break;}
            if(!isinvalid)cout<<globalspace<<i+1<<". "<<universities[i].name<<endl;
            else {
                errlist2[errcount]=i+1;
                errcount++;
            }
        }
        cout<<globalspace<<"enter choice "<<k+1<<endl;
        cout<<globalspace<<" __"<<endl<<globalspace<<"|";
        cin>>local21;
        for(int er=0;er<errcount;er++)
        if(local21==errlist2[er]){error=true;break;}
        if(notgood(&local21,1,universityCount)||error){ErrorDialog();goto looploc;}
        students[studentCount].university_order[k]=local21-1;
        }

        // list available departments
        for(int k=0;k<departmentCount;k++){
            system("cls");
            looploc2:
            errcount=0;
            error=false;
            cout<<globalspace<<"Editing department choice\n";
            for(int i=0;i<departmentCount;i++){
            bool isinvalid=false;
            for(int j=0;j<k;j++)
                if(departments[i].code==students[studentCount].department_order[j])
                    {isinvalid=true;break;}
            if(!isinvalid)cout<<globalspace<<i+1<<". "<<departments[i].name<<endl;
            else{
                errlist[errcount]=i+1;
                errcount++;
            }
        }
        cout<<globalspace<<"enter choice "<<k+1<<endl;
        cout<<globalspace<<" __"<<endl<<globalspace<<"|";
        cin>>local21;
        for(int er=0;er<errcount;er++)
        if(local21==errlist[er]){error=true;break;}
        if(notgood(&local21,0,departmentCount)||error){ErrorDialog();goto looploc2;}
        students[studentCount].department_order[k]=local21-1;
        }
        studentCount++;
        SuccessDialog("you have successfully added a new student");
        goto Managers;
        break;
    case 8://edit student
        Managers11:
            global1=-1;
            cout<<globalspace<<"Enter the id of the student.\n";
            cout<<globalspace<<" ________"<<endl<<globalspace<<"|";
            cin>>local21;
            if(notgood(&local21,100000,999999)){ErrorDialog("Invalid Id");goto Managers11;}
            for(int i=0;i<studentCount;i++)
            {
                if(local21==students[i].id)
                {global1=i; break;}
            }
            if(global1!=-1){
            Managers2:
            cout<<globalspace<<"You are Editing data for "<<students[global1].name<<endl;
            cout<<globalspace<<"1.change name\n"
            <<globalspace<<"2.change sex\n"
            <<globalspace<<"3.change id\n"
            <<globalspace<<"4.edit score\n"
            <<globalspace<<"5.edit department choice\n"
            <<globalspace<<"6.edit university choice\n"
            <<globalspace<<"7.change condition\n"
            <<globalspace<<"8.go back\n";
            cout<<globalspace<<" __"<<endl<<globalspace<<"|";
            cin>>local21;
            if(notgood(&local21,1,8)) {ErrorDialog();goto Managers2;}
            system("cls");
            switch(local21)
            {
            case 1://changing name
                Managers111:
                cin.ignore(1);
                cout<<globalspace<<"Enter full name of the student. (first_name father_name grandfather_name)\n";
                cout<<globalspace<<" ________________"<<endl<<globalspace<<"|";
                cin.getline(string1,30,'\n');
                min=0;
                for(int i=0;i<30;i++)
                    if(string1[i]==(char)32)min++;
                if(min!=2){ErrorDialog("Invalid Name!!!");goto Managers111;}
                strcpy(students[global1].name,string1);
                SuccessDialog("Student Renamed!");
                goto Managers2;
                break;
            case 2://changing sex;
                if(students[global1].sex=='F'){
                    students[global1].sex='M';
                    SuccessDialog("Sex changed to Male");}
                else {
                    students[global1].sex='F';
                    SuccessDialog("Sex changed to Female");}
                // make sex opposite of the earlier one(-_-)
                goto Managers2;
                break;
            case 3://changing id;
                Managers112:
                cout<<globalspace<<"Enter new id\n";
                cout<<globalspace<<" _______"<<endl<<globalspace<<"|";
                cin>>local21;
                if(notgood(&local21,100000,999999)){ErrorDialog();goto Managers112;}
                for(int i=0;i<studentCount;i++)
                    if(students[i].id==local21){ErrorDialog("This ID is Taken. Enter a correct ID");goto Managers112;}
                students[global1].id=local21;
                SuccessDialog("Id changed!!!");
                goto Managers2;
                break;
            case 4://editing score
                Managers113:
                cout<<globalspace<<"enter new score\n";
                cout<<globalspace<<" ____"<<endl<<globalspace<<"|";
                cin>>local21;
                if(notgood(&local21,352,700)){ErrorDialog("Invalid Score!!!\nScore must be b/n 352 and 700",2000);goto Managers113;}
                students[global1].score=local21;
                SuccessDialog("Score Changed!!!");
                goto Managers2;
                break;
            case 5://edit department choice
        // list available departments
                for(int k=0;k<departmentCount;k++){
                    system("cls");
                    looploc3:
                    errcount=0;
                    error=false;
                    cout<<globalspace<<"Editing department choice\n";
                    for(int i=0;i<departmentCount;i++){
                        bool isinvalid=false;
                        for(int j=0;j<k;j++)
                            if(departments[i].code==students[global1].department_order[j])
                                {isinvalid=true;break;}
                    if(!isinvalid)cout<<globalspace<<i+1<<". "<<departments[i].name<<endl;
                    else{
                            errlist[errcount]=i+1;
                        errcount++;
                        }
                        }
                    cout<<globalspace<<"enter choice "<<k+1<<endl;
                    cout<<globalspace<<" __"<<endl<<globalspace<<"|";
                    cin>>local21;
                    for(int er=0;er<errcount;er++)
                    if(local21==errlist[er]){error=true;break;}
                    if(notgood(&local21,0,departmentCount)||error){ErrorDialog();goto looploc3;}
                    students[global1].department_order[k]=local21-1;
        }
                SuccessDialog("Department Choice Edited!!!");
                goto Managers2;
                break;
            case 6://edit university choice
                for(int k=0;k<universityCount;k++){
                    system("cls");
                    looploc4:
                    errcount=0;
                    error=false;
                    cout<<globalspace<<"Editing university choice\n";
                    for(int i=0;i<universityCount;i++){
                        bool isinvalid=false;
                        for(int j=0;j<k;j++)
                            if(universities[i].code==students[global1].university_order[j])
                                {isinvalid=true;break;}
                        if(!isinvalid)cout<<globalspace<<i+1<<". "<<universities[i].name<<endl;
                        else{
                            errlist2[errcount]=i+1;
                            errcount++;
                        }
                    }
                cout<<globalspace<<"enter choice "<<k+1<<endl;
                cout<<globalspace<<" __"<<endl<<globalspace<<"|";
                cin>>local21;
                for(int er=0;er<errcount;er++)
                if(local21==errlist2[er]){error=true;break;}
                if(notgood(&local21,0,universityCount)||error){ErrorDialog();goto looploc4;}
                students[global1].university_order[k]=local21-1;
                }
                SuccessDialog("University Edited!!!");
                goto Managers2;
                break;
            case 7:// disability
                if(students[global1].disabled==0){
                    students[global1].disabled=1;
                    SuccessDialog("Student condition changed to 'Disabled'");}
                else {
                    students[global1].disabled=0;
                    SuccessDialog("Student condition changed to 'Normal'");}
                goto Managers2;
                break;
            case 8://go back
                global1=-1;
                goto Managers;
                break;
            default:
                goto Managers2;
            }
            }
            else {
                    ErrorDialog("There is no student with the specified ID.\nTry again!",2000);
                    goto Managers11;}
    case 9: //adding departments
        char string1[20];
        cin.ignore(1);
        system("cls");
        cout<<globalspace<<"Enter the name of the department\n";
        cout<<globalspace<<" ____________"<<endl<<globalspace<<"|";
        cin.getline(string1,21,'\n');
        strcpy(departments[departmentCount].name,string1);
        departments[departmentCount].code=departmentCount;
        departmentCount++;
        goto Managers;
        break;
    case 10: //do an allocation
        ALLOCATE_STUDENTS();
        SuccessDialog("students Allocated");
        goto Managers;
        break;
    case 11: //see allocation result
        see1:
        system("cls");
        if(!allocated)goto Managers;
        cout<<globalspace<<"here is the result\n";
        // columns ---->  student name  sex  score  university  department
        cout<<setw(25)<<"[NAME]"<<setw(5)<<"[SEX]"<<setw(8)<<"[ID]"<<setw(7)<<"[SCORE]"<<setw(20)<<"[UNIVERSITY]"<<setw(20)<<"[DEPARTMENT]"<<endl;
        for(int i=0;i<AllocatedCount;i++)
        {
            cout<<setw(25)<<result[i].Student.name<<setw(5)<<result[i].Student.sex<<setw(8)<<result[i].Student.id
            <<setw(7)<<result[i].Student.score<<setw(20)<<result[i].University.name
            <<setw(20)<<result[i].Department.name<<endl;
        }
        cout<<globalspace<<"Click ESC to continue...";
        if(getch()==27)
        goto Managers;
        else goto see1;
        break;
    case 12: //prepare report;
        if(!allocated)goto Managers;
        Managers13:
        cout<<globalspace<<"1.report by name\n"
        <<globalspace<<"2.report by university\n"
        <<globalspace<<"3.report by department\n"
        <<globalspace<<"4.go back\n";
        cout<<globalspace<<" __"<<endl<<globalspace<<"|";
        cin>>local21;
        if(notgood(&local21,1,4)){ErrorDialog();goto Managers13;}
        Sleep(1000);
        switch(local21)
        {
        case 1://report by name
            system("cls");
            report.open("report by name.txt",ios::out);
            report<<setw(25)<<"[NAME]"<<setw(5)<<"[SEX]"<<setw(8)<<"[ID]"<<setw(7)<<"[SCORE]"<<setw(20)<<"[UNIVERSITY]"<<setw(20)<<"[DEPARTMENT]"<<endl;
            for(int i=0;i<AllocatedCount;i++)
            {
                report<<setw(25)<<result[i].Student.name<<setw(5)<<result[i].Student.sex<<setw(8)<<result[i].Student.id
                <<setw(7)<<result[i].Student.score<<setw(20)<<result[i].University.name
                <<setw(20)<<result[i].Department.name<<endl;
            }
            report.close();
            SuccessDialog("Report is prepared see the file 'report by name'",2555);
            goto Managers13;
            break;
        case 2://report by university
            report.open("report by university.txt",ios::out);
            for(int uni=0;uni<universityCount;uni++)
            {
                bool hasone=false;
                for(int i=0;i<studentCount;i++)
                {
                    if(result[i].University.code==universities[uni].code){
                        hasone=true;
                        break;
                    }
                }
                if(hasone){
                report<<endl<<"    ["<<universities[uni].name<<"]"<<endl;
                for(int dep=0;dep<universities[uni].departmentCount;dep++)
                {
                    bool hasone2=false;
                    for(int kk=0;kk<studentCount;kk++)
                    {
                        if(result[kk].Department.code==universities[uni].departments[dep].code && result[kk].University.code==universities[uni].code){ hasone2=true;break;}
                    }
                if(hasone2){
                    report<<endl<<setw(20)<<"["<<departments[universities[uni].departments[dep].code].name<<"]"<<endl;
                    report<<setw(20)<<"'"<<"''''''''''''''"<<endl;
                    report<<setw(25)<<"_name_"<<setw(5)<<"_sex_"<<setw(10)<<"_ID_"<<setw(7)<<"_score_"<<endl;
                    for(int stu=0;stu<studentCount;stu++)
                    {
                        if(result[stu].University.code==universities[uni].code && result[stu].Department.code==universities[uni].departments[dep].code)
                        {
                            report<<setw(25)<<result[stu].Student.name<<setw(5)<<result[stu].Student.sex<<setw(10)<<result[stu].Student.id<<setw(7)<<result[stu].Student.score<<endl;
                        }
                    }
                }
                }
                report<<"========================================================================================";
                }
            }
            report.close();
            SuccessDialog("report is prepared.see the file 'report by university'",2000);
            system("cls");
            goto Managers13;
            break;
        case 3://report by department
            report.open("report by department.txt",ios::out);
            for(int dep=0;dep<departmentCount;dep++)
            {
                bool hasone=false;
                for(int stu=0;stu<studentCount;stu++)
                if(result[stu].Department.code==departments[dep].code){hasone=true;break;}
                if(hasone){
                report<<setw(20)<<'['<<departments[dep].name<<']'<<endl;
                for(int stu=0;stu<studentCount;stu++)
                {
                    if(result[stu].Department.code==departments[dep].code)
                    {
                        report<<setw(25)<<result[stu].Student.name<<setw(5)<<result[stu].Student.sex<<setw(10)<<result[stu].Student.id<<setw(7)<<result[stu].Student.score<<endl;
                    }
                }
                report<<"============================================================================================="<<endl;
            }
            }
            report.close();
            SuccessDialog("Report is prepared. see the file 'report by department");
            system("cls");
            goto Managers13;
            break;
        case 4:
            goto Managers;
        default:
            goto Managers13;
        }
        goto Managers;
        break;
    case 13://log out
        //handle university leakage
        if(depchoiceCount<departmentCount)
        {
            ErrorDialog("Default value is given to un configured field choices!\nplease review students field choices\nPlease wait...",4000);
            for(int i=0;i<studentCount;i++)
            {
                for(int j=depchoiceCount;j<departmentCount;j++)
                    students[i].department_order[j]=j;
            }
            depchoiceCount=departmentCount;
        }
        if(unichoiceCount<universityCount)
        {
            ErrorDialog("Default value is given to un configured university choices!\nplease review students university choices\nPlease wait...",4000);
            for(int i=0;i<studentCount;i++)
            {
                for(int j=unichoiceCount;j<universityCount;j++)
                    students[i].university_order[j]=j;
            }
            unichoiceCount=universityCount;
        }
        //write all the data back to files for future use
        campus.open("Universities.txt",ios::in);// ios::in is used for the files not to be erased if there is an error
        learner.open("Students.txt",ios::in);
        subject.open("Departments.txt",ios::in);
        if(!campus.is_open()||!learner.is_open()||!subject.is_open()) {ErrorDialog("Some files are Missing!!!\nData not Written!!!");break;}
        else
        {
            campus.close(); learner.close(); subject.close();
            campus.open("Universities.txt",ios::out); // ios::out is used after making sure the files exist
            learner.open("Students.txt",ios::out);
            subject.open("Departments.txt",ios::out);
            campus<<"These are legal universities\n";
        for(int i=0;i<universityCount;i++)
        {
           campus<<i<<'-'<<universities[i].name<<'_';
           for(int j=0;j<universities[i].departmentCount;j++){
            campus<<universities[i].departments[j].code<<'-'<<universities[i].departments[j].capacity;
            if(j==universities[i].departmentCount-1)campus<<';';
            else campus<<',';
           }
           if(i!=universityCount-1)campus<<endl;
        }
        learner<<"The following is the list of students with column order of name,sex,id,score,department choices and university choices, respectively"<<endl;
        for(int i=0;i<studentCount;i++)
        {
            learner<<students[i].name<<'_'<<students[i].sex<<'_'<<students[i].disabled<<'_'<<students[i].id<<'_'<<students[i].score<<'_';
            for(int j=0;j<departmentCount;j++)
            {
                learner<<students[i].department_order[j];
                if(j==departmentCount-1)learner<<'_';
                else learner<<'-';
            }
            for(int j=0;j<universityCount;j++)
            {
                learner<<students[i].university_order[j];
                if(j==universityCount-1)learner<<';';
                else learner<<'-';
            }
            if(i!=studentCount-1) learner<<endl;
        }
        subject<<"These are the departments with their respective codes"<<endl;
        for(int i=0;i<departmentCount;i++)
        {
            subject<<departments[i].code<<'-'<<departments[i].name;
            if(i!=departmentCount-1) subject<<endl;
        }
        SuccessDialog("Files are Updated");
        }
        break;// this ends the switch case so it returns to main//
    default:
        goto Managers;
    }
    }
}
void Student_Account(void)
{
    Student:
    system("cls");
    if(allocated) //if allocation is done
    {
        cout<<globalspace<<"Enter your ID: ";
        cout<<globalspace<<" _______"<<endl<<globalspace<<"|";
        cin>>global1;
        if(notgood(&global1,100000,999999)){
            ErrorDialog("ID must be b/n 100000 and 999999");
            goto Student;
        }
        bool found=false;
        for(int i=0;i<studentCount;i++)
        {
            if(global1==result[i].Student.id)
            {
                found=true;
                cout.setf(ios::left);
                cout<<"Dear "<<result[i].Student.name<<"."<<endl<<"based on your score which is "<<result[i].Student.score<<", "<<endl
                <<"you have been accepted at "<<result[i].University.name<<" university"
                <<" in the department of "<<result[i].Department.name<<endl
                <<"We wish you a happy university life\n";
                cout<<"\n\n\n"<<globalspace<<"Enter any key to continue...";
                getch();
                break;
            }
        }
        if(!found)
        {
            ErrorDialog("There is no student with this ID\nRetry");
        }
    }
    else // if allocation is not done
    {
            ErrorDialog("Allocation is not done yet!!!",2000);
}}
void ALLOCATE_STUDENTS(void)
{
    //do the allocation here
    cout<<globalspace<<"The default female affirmative percentage is 2.0.\n"<<globalspace<<"To change it click Enter. Any other key otherwise...";
    if(getch()==13)
    {
        loc1:
        cout<<endl<<globalspace<<"Enter percent\n";
        cout<<globalspace<<" ___"<<endl<<globalspace<<"|";
        cin>>femaleSupport;
        if(notgood())goto loc1;
    }
    AllocatedCount=0;
    //creating a copy of students and universities so as not to permanently delete the data in the files
    student* sortedstudents=new student[studentCount];
    for(int i=0;i<studentCount;i++)
    sortedstudents[i]=students[i];
    //for every female student increase her score by the support percentage.
    for(int i=0;i<studentCount;i++)
    {
        if(sortedstudents[i].sex=='F') sortedstudents[i].score+=((sortedstudents[i].score/100)*femaleSupport);
    }
    SortStudents(sortedstudents,studentCount,by_score);
    university* University=new university[universityCount];
    for(int i=0;i<universityCount;i++)
    University[i]=universities[i];
    //for every student in the list
    for(int stu=0;stu<studentCount;stu++)
    {
        bool found=false;
        // see if the department he chose is found in the universities of his choice
        for(int dep=0;dep<departmentCount;dep++)
        {
            for(int uni=0;uni<universityCount;uni++)
            {
                for(int i=0;i<University[sortedstudents[stu].university_order[uni]].departmentCount;i++)
                    {if(sortedstudents[stu].department_order[dep]==University[sortedstudents[stu].university_order[uni]].departments[i].code && University[sortedstudents[stu].university_order[uni]].departments[i].capacity>0)
                    {found=true;break;}}
                if(found){
                        // allocate the student here if the department is found at this university
                    result[stu].Student=sortedstudents[stu];
                    result[stu].Department=departments[sortedstudents[stu].department_order[dep]];
                    result[stu].University=University[sortedstudents[stu].university_order[uni]];
                    AllocatedCount++;
                    University[sortedstudents[stu].university_order[uni]].departments[sortedstudents[stu].department_order[dep]].capacity--;
                }
                if(found)break;
            } if(found)break;
        }
        if(!found) ErrorDialog("students are over limit");
    }
    for(int i=0;i<studentCount;i++)
    {
        if(result[i].Student.sex=='F') result[i].Student.score-=((result[i].Student.score/100)*femaleSupport);
    }
    allocated=true;
    delete []University;
    delete []sortedstudents;
}
void Animate2(string text,unsigned int speed,int space)
{
    string Space="";
    for(int i=0;i<space;i++) Space+=" ";
    for(unsigned int i=0;i<text.length();i++)
    {
        for(int j=0;j<4;j++)
        {
            system("cls");
            for(int q=0;q<j;q++)
                cout<<Space<<endl;
            cout<<Space;
            for(unsigned int w=0;w<i;w++)
                cout<<" ";
            cout<<text[i];
            for(int p=j;p<4;p++)
                cout<<Space<<endl;
            cout<<Space;
            for(unsigned int t=0;t<i;t++)
                    cout<<text[t];
            Sleep(speed);
        }
        Sleep(speed);
    }
    system("cls");
    for(int i=0;i<4;i++)
        cout<<Space<<endl;
    cout<<Space;
    cout<<text;
}
void ErrorDialog(string message,int delay,int space)
{       string Space="";
        for(int i=0;i<space;i++)
            Space+=" ";
        system("cls");
        system("color 4f");
        cout<<Space<<message;
        Sleep(delay);
        system("cls");
        system("color f0");
}
inline bool notgood()
{
    if(!cin){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n'); // ignores maximum possible invalid characters
    return true;
} else return false;
}
inline bool notgood(int* num,int min,int max)
{
    if(!cin||*num<min||*num>max){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); // ignores maximum possible invalid characters and numbers
        return true;
    } else return false;
}
void SuccessDialog(string message,int delay,int space)
{
    string Space="";
    for (int i=0;i<space;i++)
        Space+=" ";
    system("cls");
    system("color 2f");
    cout<<Space<<message;
    Sleep(delay);
    system("cls");
    system("color f0");
}
//returns a department with the specified code
department DepMatch(int code)
{
    for(int i=0;i<departmentCount;i++)
        if(code==departments[i].code)
        return departments[i];
    department NULLdep;
    strcpy(NULLdep.name,"NULL");
    return NULLdep;
}
// returns a department with specified code with given department list
department DepMatch(department deplist[],int depcount,int code)
{
    for(int i=0;i<depcount;i++)
        if(code==deplist[i].code)
        return deplist[i];
    department NULLdep;
    strcpy(NULLdep.name,"NULL");
    return NULLdep;
}
// password encrypt and verifier function
bool verfypassword(string keyword,string password)
{
    bool log=false;
    char key='K';
    string keycode=password;
    for(unsigned int i=0;i<password.length();i++)
        keycode[i]=password[i]^key;
    if(keycode==keyword)
        log=true;
    return log;
}
// bubble sorting
void SortStudents(student Students[],int sized,int sorttype)
{
    switch(sorttype)
    {
    case by_id:
        for(int i=sized-1;i>=0;i--)
        {
            for(int j=0;j<=i-1;j++)
                if(Students[j].id<Students[j+1].id)
                    swap(Students[j],Students[j+1]);
        }
        break;
    case by_name:
        for(int i=sized-1;i>=0;i--)
        {
            for(int j=0;j<=i-1;j++)
                if(strcmp(Students[j].name,Students[j+1].name)>0)
                    swap(Students[j],Students[j+1]);
        }
        break;
    case by_score:
        for(int i=sized-1;i>=0;i--)
        {
            for(int j=0;j<=i-1;j++)
                if(Students[j].score<Students[j+1].score)
                    swap(Students[j],Students[j+1]);
        }
        break;
    }
}
