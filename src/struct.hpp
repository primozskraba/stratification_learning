namespace strct {

    template <typename number>
    int Space<number>::vector_count() const {
        return Mat::cols();
    }

    template <typename number>
    Space<number> Map<number>::operator ()(const Space<number>& space) const {
        Space<number> result; apply(space, result);
        return result;
    }

    template <typename number>
    TimeVector<number> Map<number>::operator ()(const IVector<number>& vec) const {
        la::TimeVector<number> result(Mat::rows());  apply(vec, result);
        return result;
    }

    template <typename number>
    void Map<number>::decompose(Space<number>& kernel, Space<number>& image) const {
        Matrix<number>::decompose(kernel, image);
    }

    template <typename number>
    void Map<number>::apply(const Space<number>& space, Space<number>& result) const {
        multiply(*this, space, result);
    }

    template <typename number>
    void Map<number>::apply(const IVector<number>& vec, TimeVector<number>& result) const {
        multiply(*this, vec, result);
    }

    template <typename number>
    void Map<number>::getDomainImgTimeDiffs(std::vector<std::pair<tstep,tstep>>& intervals) const {
        assert(Matrix<number>::isReducedForm());

        const int rows = Matrix<number>::rows();
        const int cols = Matrix<number>::cols();

        if (!intervals.empty()) { intervals.clear(); }

        intervals.reserve(rows);
        for (int rowN = 0; rowN < rows; rowN++) {
            intervals.push_back({ Matrix<number>::getRowTime(rowN), tstep::INFINITY });
        }

        for (int colN = 0; colN < cols; colN++) {
            const int killedN = Matrix<number>::operator [](colN).pivotDim();
            intervals[killedN].second = Matrix<number>::getColTime(colN);
        }
    }

    template <typename number>
    void Map<number>::find(const Space<number>& domain, Map<number>& map, const Space<number>& image) {
        solve(domain, map, image);
    }

    template <typename number>
    std::ostream& operator <<(std::ostream& os, const Map<number>& map) {
        return os << static_cast<Matrix<number>>(map);
    }

    template <typename number>
    Module<number>::Module(const Map<number>& boundry) {
        boundry.decompose(generators, relations);
        Map<number>::find(generators, map, relations);
    }

    template <typename number>
    void Module<number>::getBarcode(std::vector<std::pair<tstep,tstep>>& barcode) const {
        map.getDomainImgTimeDiffs(barcode);
    }
}
