#include <bits/stdc++.h>
using namespace std;

int main() {
    ofstream fout("patients.csv");
    if (!fout.is_open()) {
        cout << "Loi tao file!\n";
        return 0;
    }

    string ho[] = {"Nguyen", "Tran", "Le", "Pham", "Hoang", "Huynh", "Phan", "Vu", "Vo", "Dang"};
    string dem[] = {"Van", "Thi", "Huu", "Xuan", "Ngoc", "Hoang", "Minh", "Thanh", "Cong", "Quoc"};
    string ten[] = {"Anh", "Binh", "Chau", "Dung", "Giang", "Hai", "Hung", "Khai", "Linh", "Mai"};
    
    srand(time(NULL));
    
    for (int i = 1; i <= 10000; i++) {
        string maBN = "BN";
        if (i < 10) maBN += "0000";
        else if (i < 100) maBN += "000";
        else if (i < 1000) maBN += "00";
        else if (i < 10000) maBN += "0";
        maBN += to_string(i);

        string hoTen = ho[rand() % 10] + " " + dem[rand() % 10] + " " + ten[rand() % 10];
        int mucDo = rand() % 5 + 1;
        
        int day = rand() % 28 + 1;
        int month = rand() % 12 + 1;
        int year = 2026;
        int hour = rand() % 12 + 7;
        int minute = rand() % 60;
        
        int r = rand() % 100;
        string trangThai = "CHO_KHAM";
        if (r >= 70 && r < 80) trangThai = "TAM_HOAN";
        else if (r >= 80 && r < 85) trangThai = "DANG_KHAM";
        else if (r >= 85) trangThai = "DA_KHAM";

        fout << maBN << "," << hoTen << "," << mucDo << "," 
             << day << "," << month << "," << year << "," 
             << hour << "," << minute << "," << 0 << "," << trangThai << "\n";
    }
    
    fout.close();
    cout << "Da tao xong 10000 vao file 'patients.csv'!\n";
    return 0;
}