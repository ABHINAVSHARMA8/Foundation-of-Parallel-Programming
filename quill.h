namespace quill{

    void init_runtime();
    void start_finish();
    void async(std::function<void()> &&lambda);
    void end_finish();
    void finalize_runtime();
}