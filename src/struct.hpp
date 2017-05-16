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
    la::TimeVector<number> Map<number>::operator ()(const la::IVector<number>& vec) const {
        la::TimeVector<number> result(Mat::rows());  apply(vec, result);
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
    void Map<number>::apply(const la::IVector<number>& vec, la::TimeVector<number>& result) const {
        multiply(*this, vec, result);
    }
}
