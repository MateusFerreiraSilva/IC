#include "../libs/test_compressed_bitvector.h"

class BitvectorTest {
    public:
        vector<bool> bitvector;
        
        BitvectorTest(int n) {
            bitvector.resize(n);
            for (ulong i = 0; i < bitvector.size(); i++)
                bitvector[i] = 0;            
        }

        uint access(uint i) {
            if (i == 0 || i > bitvector.size()) return 0;
            return bitvector[i - 1];
        }

        void fill(vector<uint> idxs) {
            for (auto idx : idxs)
                bitvector[idx - 1] = 1;
        }

        uint  rank1(uint i) {
            if (i == 0 || i > bitvector.size()) return 0;
            uint sum = 0;
            for (uint j = 1; j <= i; j++)
                sum += access(j);
            return sum;
        }
        
        uint  rank0(uint i) {
            if (i == 0 || i > bitvector.size()) return 0;
            return i - rank1(i);
        }

        uint ones() {
            return rank1(bitvector.size());
        }

        uint zeros() {
            return bitvector.size() - ones();
        }


        uint select1(uint i) {
            if (i == 0 || i > ones()) return 0;
            for (ulong j = 1; j <= bitvector.size(); j++)
                if (rank1(j) == i) return j;
            return 0; // default value 
        }

        uint select0(uint i) {
            if (i == 0 || i > zeros()) return 0;
            for (ulong j = 1; j <= bitvector.size(); j++)
                if (rank0(j) == i) return j;
            return 0; // default value
        }

        void print() {
            for (ulong i = 0; i < bitvector.size(); i++)
                cout << bitvector[i];
            cout << endl;
        }
};

void error_log_if(bool condition, uint idx, string test_name) {
    if (!condition) cout << test_name << " failed at index " << idx << endl;
}

void test_access(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (int i = 1; i <= n; i++) {
        error_log_if(bitvector_test->access(i) == bitvector->access(i), i, "access");
        assert(bitvector_test->access(i) == bitvector->access(i));
    }
    cout << "access OK" << endl;
}

void test_rank1(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (int i = 1; i <= n; i++) {
        error_log_if(bitvector_test->rank1(i) == bitvector->rank1(i), i, "rank1");
        assert(bitvector_test->rank1(i) == bitvector->rank1(i));
    }
    cout << "rank1 OK" << endl;
}

void test_rank0(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (int i = 1; i <= n; i++) {
        error_log_if(bitvector_test->rank0(i) == bitvector->rank0(i), i, "rank0");
        assert(bitvector_test->rank0(i) == bitvector->rank0(i));
    }
    cout << "rank0 OK" << endl;
}

void test_select1(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (uint i = 1; i <= bitvector_test->ones(); i++) {
        error_log_if(bitvector_test->select1(i) == bitvector->select1(i), i, "select1");
        assert(bitvector_test->select1(i) == bitvector->select1(i));
    }
    cout << "select1 OK" << endl;
}

void test_select0(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (uint i = 1; i <= bitvector_test->zeros(); i++) {
        error_log_if(bitvector_test->select0(i) == bitvector->select0(i), i, "select0");
        assert(bitvector_test->select0(i) == bitvector->select0(i));
    }
    cout << "select0 OK" << endl;
}

void compressed_bitvector_test() {
    int n = 168;
    vector<uint> idxs = {2, 7, 9, 23, 24, 25, 31, 43, 61, 62, 64, 65, 90, 91, 118, 168};
    BitvectorTest *bitvector_test = new BitvectorTest(n);
    bitvector_test->fill(idxs);
    const uint block_size = 13; // erro no 14 para frente
    const uint total_of_blocks = bitvector_test->bitvector.size() / block_size + 1;
    CompressedBitvector *bitvector = new CompressedBitvector(block_size, total_of_blocks, bitvector_test->bitvector);

    bitvector_test->print();
    bitvector->print();

    test_access(bitvector_test, bitvector, n);
    test_rank1(bitvector_test, bitvector, n);
    test_rank0(bitvector_test, bitvector, n);
    test_select1(bitvector_test, bitvector, n);
    test_select0(bitvector_test, bitvector, n);

    delete bitvector_test;
    delete bitvector;
    idxs.clear();
}

void compressed_bitvector_mem_test() {
    // cout << "Compact Bitvector\n";

    srand(time(NULL));

    const uint n = 100000 * 5;
    BitvectorTest *bitvector_test = new BitvectorTest(n);
    for (uint i = 0; i < n; i++)
        bitvector_test->bitvector.at(i) = (rand() % 100 + 1) <= 50;
    // cout << "Uncompact:\n";
    malloc_count_print_status();
    const uint block_size = 12;
    const uint block_num = bitvector_test->bitvector.size() / block_size + 1;
    CompressedBitvector *bitvector = new CompressedBitvector(block_size, block_num, bitvector_test->bitvector);
    delete bitvector_test;
    // cout << "Compact:\n";
    malloc_count_print_status();
    delete bitvector;
}