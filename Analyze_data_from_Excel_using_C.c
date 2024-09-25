#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Nhóm - 2 thành viên:
Họ và tên: Phạm Nguyễn Cao Triều    - MSSV: 22207123
Họ và tên: Huỳnh Phạm Minh Tú       - MSSV: 22207094
*/
typedef struct {
    int yb;       // YearBuilt
    int ls;       // LandSize
    int price;    // Price
    char pc[7];   // Postcode
    char slr[10]; // SellerG
} home;   //struct home lưu dữ liệu chính

typedef struct {
    char slr[10];   //slr[] mảng lưu tên 1 seller
    int count;      //lưu số nhà 1 seller bán được
} sc; //struct sc để lưu số nhà mà mỗi seller bán được

// Hàm in danh sách, để kiểm tra chương trình đọc được dữ liệu hay không
void inall(home h[], int n) {
    printf("\nYearBuilt\tLandSize\t   Price\tPostcode\tSellerG\n");  //Tiêu đề, căn chỉnh 
    for (int i = 0; i < n; i++) {
        printf("%9d\t%8d\t%10d\t%6s\t     %9s\n", h[i].yb, h[i].ls, h[i].price, h[i].pc, h[i].slr); //dữ liệu, căn chỉnh
    }
}

// Hàm đọc dữ liệu từ file CSV
void docFile(const char *filename, home h[], int *n) { //cần truyền tên file, mảng h, và địa chỉ của n(số lượng dữ liệu)
    FILE *file = fopen(filename, "r");  //mở file để đọc
    if (!file) {                        //nếu như *file không thay đổi -> không mở được file
        perror("Khong mo duoc file");
        *n = -1;
        return; //trả về hàm main, kết thúc chương trình
    }

    char line[1024];    //lưu dữ liệu dòng đầu tiên, dòng chứa tên các trường dữ liệu
    int count = 0;

    // Đọc dòng tiêu đề
    if (fgets(line, sizeof(line), file) == NULL) {  //kiểm tra đọc được dòng đầu tiên hay không
        perror("khong the doc dong dau tien");
        *n = -1;
        fclose(file);
        return;
    }

    // Đọc dữ liệu
    while (fgets(line, sizeof(line), file) && count < 10000) {  
        int price = 0, yb = 0, ls = 0;  //khai báo các biến = 0
        char pc[7] = {0}, slr[10] = {0};

        if (sscanf(line, "%*[^,],%*[^,],%*d,%*[^,],%d,%*[^,],%[^,],%*[^,],%*[^,],%6[^,],%*d,%*d,%*d,%d,%*d,%d,%*[^,],%*f,%*f,%*[^,],%*d",
                &price, slr, pc, &ls, &yb) >= 4) {//định dạng dữ liệu ở các trường để đọc dữ liệu và tìm trường bị bỏ trống
            h[count].price = price; //lưu dữ liệu giá nhà
            strncpy(h[count].slr, slr, sizeof(h[count].slr) - 1);   //sao chép tên seller vào mảng và struct home theo thứ tự
            strncpy(h[count].pc, pc, sizeof(h[count].pc) - 1);  //sao chép postcode nhà
            h[count].yb = yb;   //lấy năm xây
            h[count].ls = ls;   //lấy diện tích
        } else {    //nếu trường bị trống, cho trường đó = 0 nếu là số, và null nếu là kí tự
            h[count].price = 0; 
            h[count].slr[0] = '\0';
            h[count].pc[0] = '\0';
            h[count].yb = 0;
            h[count].ls = 0;
        }

        count++; //cập nhật số lượng dữ liệu
    }

    fclose(file);   // đóng file
    *n = count;    //lưu số lượng dữ liệu vào n
}

// Hàm xuất file CSV 
void write_csv(const char *filename, home h[], int n) {
    FILE *file = fopen(filename, "w");  //mở file để ghi
    if (!file) {
        perror("Khong mo duoc file");
        return;
    }

    fprintf(file, "YearBuilt,LandSize,Price,Postcode,SellerG\n");   //in tiêu đề

    for (int i = 0; i < n; i++) {   //ghi dữ liệu vào file
        fprintf(file, "%d,%d,%d,%s,%s\n",
                h[i].yb,
                h[i].ls,
                h[i].price,
                h[i].pc[0] != '\0' ? h[i].pc : "",  // Nếu pc rỗng, không ghi
                h[i].slr[0] != '\0' ? h[i].slr : ""); // Nếu slr rỗng, không ghi
    }
    fclose(file);
}

int min(int x, int y) {
    return (x < y) ? x : y;
}

// Hàm phân phối mảng a thành b và c
void Distribute(home a[], int n, int *pb, int *pc, int k, home b[], home c[]) {
    int i, p;
    p = *pb = *pc = 0;
    while (p < n) {
        for (i = 0; (p < n) && (i < k); i++) 
            b[(*pb)++] = a[p++];
        for (i = 0; (p < n) && (i < k); i++)
            c[(*pc)++] = a[p++];
    }
}

// Hàm hợp nhất b và c vào a
void Merge(home a[], int nb, int nc, int k, home b[], home c[]) {
    int p, pb, pc, ib, ic, kb, kc;
    p = pb = pc = 0;
    ib = ic = 0;
    while ((0 < nb) && (0 < nc)) {
        kb = min(k, nb);
        kc = min(k, nc);
        if (b[pb + ib].yb <= c[pc + ic].yb) {
            a[p++] = b[pb + ib];
            ib++;
            if (ib == kb) {
                for (; ic < kc; ic++)
                    a[p++] = c[pc + ic];
                pb += kb; pc += kc;
                ib = ic = 0;
                nb -= kb; nc -= kc;
            }
        } else {
            a[p++] = c[pc + ic];
            ic++;
            if (ic == kc) {
                for (; ib < kb; ib++)
                    a[p++] = b[pb + ib];
                pb += kb; pc += kc;
                ib = ic = 0;
                nb -= kb; nc -= kc;
            }
        }
    }
}

// Hàm Merge Sort
void MergeSort(home a[], int n) {
    home b[10000], c[10000];
    int pb, pc;
    int i, k = 1;
    do {
        Distribute(a, n, &pb, &pc, k, b, c);
        Merge(a, pb, pc, k, b, c);
        k *= 2;
    } while (k < n);
}


// Hàm đếm số lượng ký tự postcode để sắp xếp
int getMaxLength(home h[], int n) {
    int maxLen = 0;
    for (int i = 0; i < n; i++) {
        int len = strlen(h[i].pc);
        if (len > maxLen) {
            maxLen = len;
        }
    }
    return maxLen;
}

// Hàm Radix Sort cho Postcode
void count_Postcode(home h[], int n, int exp) {

    home *output = (home *)malloc(n * sizeof(home));
    int count[10] = {0};

    for (int i = 0; i < n; i++) {
        int index = (h[i].pc[exp] - '0');
        count[index]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int index = (h[i].pc[exp] - '0');
        output[count[index] - 1] = h[i];
        count[index]--;
    }

    for (int i = 0; i < n; i++) {
        h[i] = output[i];
    }

    free(output);
}

// Hàm Radix Sort
void sort_postcode(home arr[], int n) {
    int maxLen = getMaxLength(arr, n);

    for (int exp = maxLen - 1; exp >= 0; exp--) {
        count_Postcode(arr, n, exp);
    }
}

// Hàm in danh sách seller + số lượng nhà bán 
void insellers(sc slrq[], int scount) {
    printf("\n+----------------+------------+\n");
    printf("|    SellerG     | Homes Sold |\n");
    printf("+----------------+------------+\n");

    for (int i = 0; i < scount; i++) {
        printf("| %-14s | %10d |\n", slrq[i].slr, slrq[i].count);
    }

    printf("+----------------+------------+\n");
}

// Hàm tìm seller bán nhiều nhà nhất
void bestsellers(home h[], int n) {
    sc slrq[1000];  //mảng slrq[] lưu các seller vào struct sc theo tên
    int scount = 0; //biến đếm số lượng seller trong mảng slqr[]

    // Đếm số lượng nhà bán của từng seller
    for (int i = 0; i < n; i++) {   //dò từ đầu tới hết dữ liệu
        int f = 0;  //biến đánh dấu 1 seller nhất định có trong mảng slrq[] hay chưa, f=0
        for (int j = 0; j < scount; j++) {  //khi số lượng seller tìm thấy > 0, một vòng lặp dò từ đầu danh sách đến tiến độ dò hiện tại
            if (strcmp(slrq[j].slr, h[i].slr) == 0) {  //nếu tên của seller ở vị trí i (hiện tại) trùng với tên seller ở vị trí j(trước đó)
                slrq[j].count++;    //tăng 1 đơn vị nhà mà seller đó bán được
                f = 1;  //cập nhật f=1 đánh dấu seller đó đã xuất hiện, đã có trong danh sách slrq[]
                break;  //hủy vòng lặp j
        }}

        if (!f) {   //nếu như biến f không thay đổi (f=0) thì seller ở vị trí i(hiện tại) là seller mới chưa có trong danh sách slrq[]
            strcpy(slrq[scount].slr, h[i].slr); //copy tên của seller mới vào vị trí kế tiếp của danh sách slrq[]
            slrq[scount].count = 1; //số nhà seller đó bán = 1
            scount++;   //cập nhật số seller trong danh sách slrq[]
        }}

    // Tìm số lượng nhà bán tối đa
    int maxCount = 0;   //max=0
    for (int i = 0; i < scount; i++) {  //dò trong danh sách slrq[]
        if (slrq[i].count > maxCount) { //tìm max, qua số nhà bán được của mỗi seller đã được lưu trong struct sc
            maxCount = slrq[i].count;   //sau mỗi vòng lặp biến max được cập nhật liên tục, tới khi kết thúc
    }}
    insellers(slrq, scount);
    // In tất cả các seller bán được số lượng nhà tối đa
    printf("\nSeller co so nha ban duoc nhieu nhat: \n"); //in ra số nhà bán được nhiều nhất của 1 seller
    for (int i = 0; i < scount; i++) {
        if (slrq[i].count == maxCount) {
            printf("Seller: %s\tSo luong: %d\n", slrq[i].slr, slrq[i].count); //vinh danh tên seller+nhà bán được 
}}}

int main() {
    home h[10000];  //mảng h lưu 10k dữ liệu
    int n = 0, c;   //n là biến số lượng, có thể n = 10000

    docFile("melb_data.csv", h, &n); //gọi hàm đọc file: melb_data.csv, truyền mảng h và địa chỉ n

    if (n < 0) {    //nếu sau khi đọc n không tăng thì kết thúc chương trình
        return 1;
    }

    do {    //menu
        printf("\nNhap lua chon\n");
        printf("------------------------Nhap 0 de thoat--------------------------\n");
        printf("1. In va xuat danh sach de kiem tra\n");
        printf("2. Sap xep va xuat danh sach theo nam xay dung tu cu den moi nhat\n");
        printf("3. Sap xep va xuat danh sach theo postcode tu nho den lon\n");
        printf("4. Tim seller ban duoc nhieu nha nhat\n");
        printf("Moi nhap: ");
        scanf("%d", &c);
        printf("\n");
        //thực hiện theo yêu cầu menu
        if (c == 0) {printf("ket thuc\n");}     //kết thúc chương trình
        if (c == 1) {inall(h, n); write_csv("output.csv", h, n);}   // in ra và xuất danh sách để kiểm tra dữ liệu đọc, chức năng inall chạy bình thường và hơi lâu 
        if (c == 2) {MergeSort(h,n);write_csv("YearBuilt.csv", h, n);} //sắp xếp theo năm và xuất danh sách
        if (c == 3) {sort_postcode(h, n); write_csv("Postcode.csv", h, n);}  //sắp xếp theo postcode và xuất danh sách
        if (c == 4) {bestsellers(h, n);}    //tìm người bán được nhiều nhà nhất
    } while (c != 0);

    return 0;
}
