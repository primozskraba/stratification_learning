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
    typename Map<number>::Vec Map<number>::operator ()(const Vec& vec) const {
        Vec result(Mat::rows());  apply(vec, result);
        return result;
    }

    template <typename number>
    void Map<number>::decompose(Space<number>& kernel, Space<number>& image) const {
        la::Matrix<number>::decompose(kernel, image);
    }

    template <typename number>
    void Map<number>::apply(const Space<number>& space, Space<number>& result) const {
        multiply(*this, space, result);
    }

    template <typename number>
    void Map<number>::apply(const Vec& vec, Vec& result) const {
        multiply(*this, vec, result);
    }
}
