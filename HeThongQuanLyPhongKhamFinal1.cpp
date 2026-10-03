#include <bits/stdc++.h>
using namespace std;

#define el "\n"

const int maxQueue = 1000000;
void ClearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

typedef long long ll;

struct Date {
    int day, month, year; 
    int hour, minute; 

    Date() {
        day = 0; month = 0; year = 0;
        hour = 0; minute = 0;
    }

    Date(int gio, int phut, int ngay, int thang, int nam) {
        hour = gio; minute = phut;
        day = ngay; month = thang; year = nam;
    }
    
    bool operator<(const Date& other) const {
        if(year != other.year) return year < other.year;
        if(month != other.month) return month < other.month;
        if(day != other.day) return day < other.day;
        if(hour != other.hour) return hour < other.hour;
        return minute < other.minute;
    }

    void PrintDate() {
        cout << setfill('0') << setw(2) << hour << ":" 
             << setfill('0') << setw(2) << minute << " ngay " 
             << setfill('0') << setw(2) << day << "/" 
             << setfill('0') << setw(2) << month << "/" 
             << year << el;
    }

    friend istream& operator>>(istream& in, Date& d) {
        char sepTime, sepDate1, sepDate2;
        
        cout << "Nhap thoi gian (HH:MM): ";
        in >> d.hour >> sepTime >> d.minute; 

        cout << " Nhap ngay thang (DD/MM/YYYY): ";
        in >> d.day >> sepDate1 >> d.month >> sepDate2 >> d.year;

        bool formatError = in.fail() || sepTime != ':' || sepDate1 != '/' || sepDate2 != '/';
        bool timeError = (d.hour < 0 || d.hour > 23) || (d.minute < 0 || d.minute > 59);
        bool dateError = false;
        if (d.year < 1900 || d.month < 1 || d.month > 12) {
            dateError = true;
        } else {
            int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            
            if ((d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0)) {
                daysInMonth[2] = 29;
            }

            if (d.day < 1 || d.day > daysInMonth[d.month]) {
                dateError = true; 
            }
        }
        if(formatError || timeError || dateError) {
            in.clear();
            in.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nLOI: Nhap sai dinh dang! Vui long dung dung dau ':' cho gio va '/' cho ngay.\n";
            cout << "Goi y: Gio (0-23), Phut (0-59), Ngay thang phai co thuc.\n";
            d.day = d.month = d.year = d.hour = d.minute = 0;
        }
        return in;
        
    }
};

enum TrangThai
{
    CHO_KHAM,
    TAM_HOAN,
    DANG_KHAM,
    DA_KHAM
};

struct Patient
{
    string maBN;
    string HoTen;

    int MucDoKhanCap;
    Date ThoiDiemDangKy;

    TrangThai trangThai;
    bool isPriority;

    int heapIndex;

    Patient() {
        maBN = "";
        HoTen = "";
        MucDoKhanCap = 0;
        ThoiDiemDangKy = Date();
        
        isPriority = false;
        heapIndex = -1;
    }

    Patient(string id, string name, int urgency, Date timestamp) {
        maBN = id;
        HoTen = name;
        MucDoKhanCap = urgency;
        ThoiDiemDangKy = timestamp;

        trangThai = CHO_KHAM;
        isPriority = false;

        heapIndex = -1;
    }

    string EnumToString(TrangThai t) {
        if (t == CHO_KHAM) return "CHO_KHAM";
        if (t == TAM_HOAN) return "TAM_HOAN";
        if (t == DANG_KHAM) return "DANG_KHAM";
        return "DA_KHAM";
    }

    void PrintPatientInfo() {
        cout << "Ma benh nhan: " << maBN << el;
        cout << "Ten benh nhan: " << HoTen << el;
        cout << "Muc do khan cap cua benh nhan: " << MucDoKhanCap << el;
        cout << "Trang thai benh nhan: " << EnumToString(trangThai) << el;
        cout << "Thoi diem dang ky kham: ";
        ThoiDiemDangKy.PrintDate();
    }

};

struct HashTable {
    vector < vector < pair<string, Patient*> > > myBucKet;
    int Size = 0;

    HashTable(int size = 10000)
    {
        myBucKet.resize(size);
    }

    int hashFunction(string& key){
        ll hash = 0;

        for(char c : key){
            hash = hash * 31 + c;
        }

        return hash % myBucKet.size();
    }
    
    Patient* find(string& key){
        int HashIndex = hashFunction(key);

        for(auto& x : myBucKet[HashIndex]){
            if(x.first == key)  return x.second;
        }

        return nullptr;
    }

    void Raw_insert(string& key, Patient* value){
        int HashIndex = hashFunction(key);

        if(find(key) == nullptr){
            myBucKet[HashIndex].push_back({key, value});
            Size++;
        }
    }

    bool erase(string& key){
        int HashIndex = hashFunction(key);
        for(int i = 0; i < myBucKet[HashIndex].size(); i++){
            if(myBucKet[HashIndex][i].first == key){
                myBucKet[HashIndex].erase(myBucKet[HashIndex].begin() + i);
                Size--;
                return true;
            }
        }
        return false;
    }

    void Resize(int newSize){
        vector < vector < pair<string, Patient*> > > old_myBucKet = myBucKet;
        
        myBucKet.clear();
        myBucKet.resize(newSize);

        Size = 0;
        for(auto& BucKet : old_myBucKet){
            for(auto& p : BucKet){
                Raw_insert(p.first, p.second);
            }
        }
    }

    bool insert(string& key, Patient* value){
        int HashIndex = hashFunction(key);

        if(find(key) == nullptr){
            myBucKet[HashIndex].push_back({key, value});
            Size++;

            if((double) Size / myBucKet.size() > 0.75){
                Resize(myBucKet.size() * 2);
            }
            return true;
        }

        return false;
    }

};

struct maxHeap {
    int max_size = 100;
    Patient** a = new Patient* [max_size + 1];
    int size = 0;

    maxHeap(){
        size = 0;
    }
    
    void Resize(int new_max){
        Patient** new_a = new Patient*[new_max + 1];
        for (int i = 1; i <= size; i++) {
            new_a[i] = a[i];
        }
        delete[] a;
        a = new_a;
        max_size = new_max;
    }
    
    void heapifyUp(int currIndex){
        int parentIndex = currIndex / 2;

        while(parentIndex > 0 and bigger(currIndex, parentIndex)){
            Swap(parentIndex, currIndex);

            currIndex = parentIndex;
            parentIndex = currIndex / 2;
        }
    }

   void heapifyDown(int currIndex){
        while(currIndex * 2 <= size){
            int leftChildIndex = currIndex * 2;
            int rightChildIndex = leftChildIndex + 1;
            int biggerChildIndex = leftChildIndex;

            if(rightChildIndex <= size and bigger(rightChildIndex, leftChildIndex)){
                biggerChildIndex = rightChildIndex;
            }

            if(bigger(biggerChildIndex, currIndex)){
                Swap(currIndex, biggerChildIndex);
                currIndex = biggerChildIndex;
            }
            else{
                break;
            }
        }
    }

    Patient* peek(){
        if(!isEmpty()){
            return a[1];
        }

        return nullptr;
    }

    void Swap(int i, int j){
        swap(a[i], a[j]);

        a[i]->heapIndex = i;
        a[j]->heapIndex = j;
    }

    bool bigger(int i, int j){

        if(a[i]->isPriority == a[j]->isPriority){
            if(a[i]->MucDoKhanCap == a[j]->MucDoKhanCap){
                return a[i]->ThoiDiemDangKy < a[j]->ThoiDiemDangKy;
            }
            return a[i]->MucDoKhanCap > a[j]->MucDoKhanCap;
        }
        
        return a[i]->isPriority > a[j]->isPriority;
    }


    bool isEmpty(){
        return size <= 0;
    }

    void add(Patient* v){
        size++;
        a[size] = v;
        v->heapIndex = size;

        if (size >= max_size) {
            Resize(max_size * 2);
        }

        int currIndex = size;
        heapifyUp(currIndex);
    }

    Patient* poll(){
        if(isEmpty()){
            return nullptr;
        }

        Patient* root = a[1];
        root->heapIndex = -1;
        
        if(size > 1){
            a[1] = a[size];
            a[1]->heapIndex = 1;
        }

        size--;

        if (size > 0) {
            heapifyDown(1);
        }

        return root;
    }

    void remove(Patient* v){

        if(isEmpty()) return;

        int currIndex = v->heapIndex;

        if(currIndex == -1){
            return;
        }

        if(currIndex < 1 or currIndex > size or a[currIndex] != v){
            return;
        }

        if(currIndex == size){
            a[currIndex] = nullptr;
            size--;
            v->heapIndex = -1;
            return;
        }

        a[currIndex] = a[size];
        a[currIndex]->heapIndex = currIndex;
        
        size--;
        v->heapIndex = -1;

        if(currIndex > 1 and bigger(currIndex, currIndex / 2))  heapifyUp(currIndex);
        else heapifyDown(currIndex);
    }

    ~maxHeap(){
        delete[] a;
    }
};

struct PhongKham {
    HashTable myHashTable;
    maxHeap myHeap;
    vector < Patient* > ListPatient;

    ~PhongKham(){
        for(auto& p : ListPatient){
            delete p;
        }

        ListPatient.clear();
    }

    bool DangKyKham(string& id, string& HoTen, int& MucDoKhanCap, Date ThoiDiemDangKy){
        if(myHashTable.find(id) != nullptr)  return false;
        else{
            Patient* p = new Patient(id, HoTen, MucDoKhanCap, ThoiDiemDangKy);

            if(!myHashTable.insert(id, p))
            {
                delete p;
                return false;
            }

            ListPatient.push_back(p);
            myHeap.add(p);

            return true;
        }
    }

    Patient* TraCuuHoSo(string& id){
        return myHashTable.find(id);
    }

    bool HuyLuotKham(string& id){
        Patient* p = myHashTable.find(id);

        if(p == nullptr || p->trangThai != CHO_KHAM) return false;
        
        myHashTable.erase(id);
        myHeap.remove(p);

        for(int i = 0; i < ListPatient.size(); i++){
            if(ListPatient[i] == p){
                ListPatient.erase(ListPatient.begin() + i);
                break;
            }
        }

        delete p;
        return true;
    }

    Patient* GoiBNTiepTheo(Patient*& BNDangKham){
        if(BNDangKham != nullptr && BNDangKham->trangThai == DANG_KHAM){
            BNDangKham->trangThai = DA_KHAM;
        }

        Patient* nextPatient = myHeap.poll();

        if(nextPatient != nullptr){
            nextPatient->trangThai = DANG_KHAM;
            BNDangKham = nextPatient;
        }
        else{
            BNDangKham = nullptr;
        }
        return nextPatient;
    }

    bool TamHoan(Patient*& BNDangKham){
        if(BNDangKham == nullptr || BNDangKham->trangThai != DANG_KHAM) return false;

        Patient* p = BNDangKham;
        p->isPriority = true;
        p->trangThai = TAM_HOAN;
        BNDangKham = nullptr;

        return true;
    }

    bool TiepTucKham(string& id){
        Patient* p = myHashTable.find(id);

        if (p == nullptr) return false;

        if(p->trangThai != TAM_HOAN) return false;

        p->trangThai = CHO_KHAM;
        myHeap.add(p);

        return true;
    }

    bool CapNhatMucDoUuTien(string& id, int& newMucDoKhanCap){
        Patient* p = myHashTable.find(id);

        if(p == nullptr || p->MucDoKhanCap == newMucDoKhanCap || p->trangThai != CHO_KHAM || newMucDoKhanCap < 0)  return false;

        myHeap.remove(p);
        p->MucDoKhanCap = newMucDoKhanCap;
        myHeap.add(p);

        return true;
    }

    int amountWaiting(){
        return myHeap.size;
    }
};

struct Persistence {
    static string EnumToString(TrangThai t) {
        if (t == CHO_KHAM) return "CHO_KHAM";
        if (t == TAM_HOAN) return "TAM_HOAN";
        if (t == DANG_KHAM) return "DANG_KHAM";
        return "DA_KHAM";
    }

    static TrangThai StringToEnum(const string& s) {
        if (s == "TAM_HOAN") return TAM_HOAN;
        if (s == "DANG_KHAM") return DANG_KHAM;
        if (s == "DA_KHAM") return DA_KHAM;
        return CHO_KHAM;
    }

    static void SaveToCSV(PhongKham& pk, const string& filename = "patients.csv") {
        ofstream fout(filename);
        if (!fout.is_open()) {
            cout << "Loi: Khong the mo file de ghi!\n";
            return;
        }
        
        for (Patient* p : pk.ListPatient) {
            fout << p->maBN << ","
                 << p->HoTen << ","
                 << p->MucDoKhanCap << ","
                 << p->ThoiDiemDangKy.day << ","
                 << p->ThoiDiemDangKy.month << ","
                 << p->ThoiDiemDangKy.year << ","
                 << p->ThoiDiemDangKy.hour << ","
                 << p->ThoiDiemDangKy.minute << ","
                 << p->isPriority << ","
                 << EnumToString(p->trangThai) << "\n";
        }
    
        fout.close();
        cout << "Luu du lieu vao file '" << filename << "' thanh cong!\n";
    }

    static void LoadFromCSV(PhongKham& pk, const string& filename = "patients.csv") {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cout << "Chua co du lieu cu, khoi tao danh sach rong.\n";
            return;
        }

        string line;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string maBN, hoTen, mucDoStr, ngayStr, thangStr, namStr, gioStr, phutStr, trangThaiStr, isPriorityStr;

            getline(ss, maBN, ',');
            getline(ss, hoTen, ',');
            getline(ss, mucDoStr, ',');
            getline(ss, ngayStr, ',');
            getline(ss, thangStr, ',');
            getline(ss, namStr, ',');
            getline(ss, gioStr, ',');
            getline(ss, phutStr, ',');
            getline(ss, isPriorityStr, ',');
            getline(ss, trangThaiStr, ',');

            int mucDo = stoi(mucDoStr);
            Date thoiDiem(stoi(gioStr), stoi(phutStr),  stoi(ngayStr), stoi(thangStr), stoi(namStr));
            
            if(!pk.DangKyKham(maBN, hoTen, mucDo, thoiDiem)) continue;

            Patient* p = pk.TraCuuHoSo(maBN);
            if (p != nullptr) {
                p->isPriority = stoi(isPriorityStr);
                p->trangThai = StringToEnum(trangThaiStr);
                
                pk.myHeap.remove(p);

                if(p->trangThai == CHO_KHAM){
                    pk.myHeap.add(p);
                }
            }
        }
        fin.close();
        cout << "Nap du lieu tu file thanh cong!\n";
    }
};

void Menu()
{
    cout << el;
    cout << "==========================================" << el;

    cout << "   HE THONG QUAN LY PHONG KHAM" << el;

    cout << "==========================================" << el;

    cout << "1. Dang ky kham" << el;
    cout << "2. Tra cuu ho so" << el;
    cout << "3. Kiem tra qua tai" << el;
    cout << "4. Goi benh nhan tiep theo" << el;
    cout << "5. Huy luot kham" << el;
    cout << "6. Tam hoan" << el;
    cout << "7. Tiep tuc kham" << el;
    cout << "8. Cap nhat muc do khan cap" << el;
    cout << "9. Hien thi danh sach" << el;
    cout << "10. Luu du lieu" << el;
    cout << "0. Thoat" << el;

    cout << "==========================================" << el;
}

int main(){

    PhongKham myDS;
    Patient* BNDangKham = nullptr;

    cout << "Dang khoi tao he thong...\n";
    Persistence::LoadFromCSV(myDS);

    while(1){
        Menu();
        int LuaChon;

        string id;
        string name;

        int urgency;
        Date timestamp;

        cin >> LuaChon;
        if(cin.fail()) {
            cout << "Vui long nhap so hop le!\n";
            ClearCin();
            continue;
        }

        if(LuaChon == 1){
            if(myDS.amountWaiting() >= maxQueue) {
                cout << "CANH BAO: Phong kham dang qua tai! Khong the nhan them benh nhan luc nay.\n";
                continue;
            }

            cout << "Nhap ma BN: ";
            cin >> id;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Nhap ho ten: ";
            getline(cin, name);


            cout << "Nhap muc do khan cap: ";
            cin >> urgency;

            cout << "Nhap thoi diem dang ky:\n ";
            cin >> timestamp;

            if(!myDS.DangKyKham(id, name, urgency, timestamp)){
                cout << "Benh nhan da co trong danh sach kham benh" << el;
            }
            else{
                cout << "Dang ky kham thanh cong" << el;;
            }            
        }

        if(LuaChon == 2){
            cout << "Nhap ma BN can tra cuu: ";
            cin >> id;
            
            Patient* p = myDS.TraCuuHoSo(id);
            if(p != nullptr){
                p->PrintPatientInfo();
            }
            else{
                cout << "Khong tim thay benh nhan" << el;
            }
        }

        if(LuaChon == 3){
            cout << "So luong benh nhan dang cho kham la: " << myDS.amountWaiting() << el;
            
            if(myDS.amountWaiting() >= maxQueue){
                cout << "CANH BAO: Phong kham dang trong tinh trang QUA TAI!" << el;
                cout << "Goi y: Nen ngung tiep nhan dang ky moi." << el;
            }
            else{
                cout << "Tinh trang phong kham binh thuong. (Chua qua tai)" << el;
            }
        }

        if(LuaChon == 4){
            Patient* p = myDS.GoiBNTiepTheo(BNDangKham);

            if(p != nullptr){
                p->PrintPatientInfo();
            }
            else{
                cout << "Khong co benh nhan nao trong danh sach cho" << el;
            }
        }

        if(LuaChon == 5){
            cout << "Nhap ma BN can huy kham: ";
            cin >> id;

            if(myDS.HuyLuotKham(id)){
                cout << "Huy luot kham thanh cong!" << el;
            }
            else{
                cout << "Khong tim thay benh nhan de huy!" << el;
            }
        }
        
        if(LuaChon == 6){
            if(myDS.TamHoan(BNDangKham)){
                cout << "Tam hoan thanh cong\n";
            }
            else{
                cout << "Loi: Chua co benh nhan nao dang duoc kham!\n";
            }
        }

        if(LuaChon == 7){
            cout << "Nhap ma BN can tiep tuc kham: ";
            cin >> id;

            if(myDS.TiepTucKham(id)){
                cout << "Benh nhan da duoc dua vao hang cho!" << el;
            }
            else{
                cout << "Benh nhan khong the dua vao hang cho (Khong tim thay hoac dang cho roi)" << el;
            }
        }

        if(LuaChon == 8){
            int MucDoMoi;

            cout << "Nhap ma benh nhan can cap nhat: ";
            cin >> id;

            cout << el;

            cout << "Nhap muc do khan cap can cap nhat: ";
            cin >> MucDoMoi;

            cout << el;

            myDS.CapNhatMucDoUuTien(id, MucDoMoi);
        }
        
        if(LuaChon == 9){
            cout << "\n========== DANH SACH HO SO BENH NHAN =========\n";
            if(myDS.ListPatient.empty()) {
                cout << "He thong hien chua co benh nhan nao!\n";
            } else {
                for(Patient* p : myDS.ListPatient) {
                    p->PrintPatientInfo();
                }
            }
            cout << "\n";
        }

        if(LuaChon == 10){
            if(BNDangKham != nullptr){
                cout << "Van con benh nhan dang kham. Hay TamHoan sau do moi luu du lieu";
                continue;
            }
            Persistence::SaveToCSV(myDS);
        }

        if(LuaChon == 0){
            if(BNDangKham != nullptr){
                cout << "Van con benh nhan dang kham. Hay TamHoan sau do moi luu va thoat";
                continue;
            }
            cout << "Dang luu du lieu truoc khi thoat...\n";
            Persistence::SaveToCSV(myDS);
            break;
        }   
    }
}