/*
   StreamDAB Integration Module for etisnoop
   Thailand DAB+ Broadcasting System - Phase 4

   Real-time ETSI compliance monitoring and reporting integration
   WebSocket communication with StreamDAB-ComplianceMonitor
*/

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <nlohmann/json.hpp>
#include "thai_analysis.hpp"

using json = nlohmann::json;

namespace StreamDAB {

// ETSI Standards compliance tracking
enum class ETSIStandard {
    EN_302_077 = 0,  // RF Equipment Standard
    EN_300_401 = 1,  // Core DAB Standard  
    TS_102_563 = 2,  // DAB+ Audio Coding
    TS_101_756 = 3,  // Character Sets (Thai Profile)
    TR_101_496_3 = 4, // Broadcast Network Implementation
    TS_101_499 = 5,  // SlideShow User Application
    TS_102_818 = 6,  // Service Programme Information
    TS_103_551 = 7,  // TPEG Services
    TS_103_176 = 8   // Service Information Features
};

// Compliance violation severity levels
enum class ViolationSeverity {
    INFO = 0,       // Informational, no action required
    WARNING = 1,    // Minor issue, should be addressed
    ERROR = 2,      // Compliance violation, must be fixed
    CRITICAL = 3    // Serious violation, immediate action required
};

// Individual compliance check result
struct ComplianceResult {
    ETSIStandard standard;
    std::string check_name;
    std::string description;
    ViolationSeverity severity;
    bool passed;
    double score;              // 0-100 compliance score
    std::string details;       // Detailed explanation
    std::string recommendation; // Suggested fix
    std::chrono::system_clock::time_point timestamp;
    std::map<std::string, std::string> metadata; // Additional data
};

// Overall ETI analysis report
struct ETIAnalysisReport {
    std::string eti_filename;
    std::chrono::system_clock::time_point analysis_time;
    
    // Overall statistics
    double overall_compliance_score;    // 0-100 average across all standards
    int total_frames_analyzed;
    int total_violations_found;
    
    // Per-standard results
    std::map<ETSIStandard, std::vector<ComplianceResult>> standard_results;
    
    // Thai language specific results
    Thai::ThaiMetadata thai_analysis;
    Thai::ComplianceLevel thai_compliance_level;
    
    // Performance metrics
    std::chrono::milliseconds analysis_duration;
    size_t memory_usage_bytes;
    
    // Recommendations and summary
    std::vector<std::string> critical_issues;
    std::vector<std::string> recommendations;
    std::string executive_summary;
};

// Real-time streaming configuration
struct StreamingConfig {
    std::string compliance_monitor_url = "http://localhost:8002";
    std::string websocket_endpoint = "/ws/etisnoop";
    bool enable_realtime_streaming = false;
    bool enable_batch_reporting = true;
    int reporting_interval_seconds = 30;
    bool enable_thai_analysis_streaming = true;
    bool enable_government_reporting = false;
    std::string government_api_endpoint = "";
    std::string api_key = "";
};

// WebSocket client for real-time communication
class WebSocketClient {
public:
    WebSocketClient(const std::string& url);
    ~WebSocketClient();
    
    bool connect();
    void disconnect();
    bool is_connected() const;
    
    // Message sending
    bool send_json(const json& data);
    bool send_compliance_update(const ComplianceResult& result);
    bool send_thai_analysis(const Thai::ThaiMetadata& thai_data);
    bool send_analysis_report(const ETIAnalysisReport& report);
    
    // Callback registration
    void set_message_handler(std::function<void(const json&)> handler);
    void set_connection_handler(std::function<void(bool)> handler);

private:
    std::string url_;
    std::atomic<bool> connected_;
    std::thread client_thread_;
    std::function<void(const json&)> message_handler_;
    std::function<void(bool)> connection_handler_;
    mutable std::mutex send_mutex_;
    
    void client_loop();
    void handle_message(const std::string& message);
};

// HTTP client for REST API communication
class HTTPClient {
public:
    HTTPClient(const std::string& base_url, const std::string& api_key = "");
    
    // REST API operations
    bool post_compliance_result(const ComplianceResult& result);
    bool post_analysis_report(const ETIAnalysisReport& report);
    bool post_thai_analysis(const Thai::ThaiMetadata& thai_data);
    
    // Government reporting
    bool submit_government_report(const ETIAnalysisReport& report);
    bool get_compliance_thresholds(std::map<ETSIStandard, double>& thresholds);
    
    // Health check
    bool health_check();

private:
    std::string base_url_;
    std::string api_key_;
    
    std::string make_request(const std::string& endpoint, const std::string& method, 
                           const json& data = json{});
};

// ETSI Standards Analyzer - Enhanced validation engine
class ETSIStandardsAnalyzer {
public:
    ETSIStandardsAnalyzer();
    
    // Standard-specific validation methods
    std::vector<ComplianceResult> validate_en_300_401(const uint8_t* eti_frame, size_t length);
    std::vector<ComplianceResult> validate_ts_102_563(const uint8_t* audio_data, size_t length);
    std::vector<ComplianceResult> validate_ts_101_756(const std::string& text_data);
    std::vector<ComplianceResult> validate_ts_101_499(const uint8_t* mot_data, size_t length);
    std::vector<ComplianceResult> validate_ts_102_818(const uint8_t* spi_data, size_t length);
    std::vector<ComplianceResult> validate_ts_103_551(const uint8_t* tpeg_data, size_t length);
    std::vector<ComplianceResult> validate_ts_103_176(const uint8_t* si_data, size_t length);
    
    // Comprehensive analysis
    ETIAnalysisReport analyze_complete_eti(const std::string& filename, const uint8_t* data, size_t length);
    
    // Configuration
    void set_validation_strictness(double strictness); // 0.0 = lenient, 1.0 = strict
    void enable_thai_validation(bool enable);
    void set_thai_analyzer(std::shared_ptr<Thai::ThaiAnalysisEngine> analyzer);

private:
    double validation_strictness_;
    bool thai_validation_enabled_;
    std::shared_ptr<Thai::ThaiAnalysisEngine> thai_analyzer_;
    
    // Standard-specific helpers
    bool validate_frame_structure(const uint8_t* frame, size_t length);
    bool validate_fig_structure(const uint8_t* fig_data, size_t length);
    bool validate_service_organization(const uint8_t* data, size_t length);
    double calculate_audio_quality_score(const uint8_t* audio_data, size_t length);
    
    // Utility methods
    std::string get_standard_name(ETSIStandard standard);
    ViolationSeverity get_severity_for_score(double score);
    ComplianceResult create_result(ETSIStandard standard, const std::string& check_name,
                                 bool passed, double score, const std::string& details);
};

// Government reporting module for Thailand DAB+ compliance
class GovernmentReporter {
public:
    GovernmentReporter(const std::string& nbtc_api_endpoint, const std::string& api_key);
    
    // NBTC compliance reporting
    bool submit_daily_compliance_report(const std::vector<ETIAnalysisReport>& reports);
    bool submit_violation_incident_report(const ComplianceResult& violation);
    bool submit_thai_language_compliance_report(const Thai::ThaiMetadata& thai_analysis);
    
    // Report formatting for government standards
    json format_nbtc_compliance_report(const ETIAnalysisReport& report);
    json format_incident_report(const ComplianceResult& violation);
    json format_thai_compliance_report(const Thai::ThaiMetadata& thai_analysis);

private:
    std::string nbtc_api_endpoint_;
    std::string api_key_;
    std::unique_ptr<HTTPClient> http_client_;
    
    bool authenticate_with_government_api();
    std::string generate_report_id();
};

// Main StreamDAB Integration Engine
class StreamDABIntegrationEngine {
public:
    StreamDABIntegrationEngine(const StreamingConfig& config);
    ~StreamDABIntegrationEngine();
    
    bool initialize();
    void shutdown();
    
    // Real-time analysis integration
    void analyze_and_report_frame(const uint8_t* eti_frame, size_t length);
    void analyze_and_report_file(const std::string& filename);
    
    // Batch reporting
    void submit_analysis_report(const ETIAnalysisReport& report);
    void submit_thai_analysis(const Thai::ThaiMetadata& thai_data);
    
    // Configuration management
    void update_streaming_config(const StreamingConfig& config);
    StreamingConfig get_streaming_config() const;
    
    // Statistics and monitoring
    int get_reports_sent() const;
    int get_violations_detected() const;
    double get_average_compliance_score() const;
    std::vector<ComplianceResult> get_recent_violations() const;
    
    // Government integration
    void enable_government_reporting(const std::string& api_endpoint, const std::string& api_key);
    void disable_government_reporting();

private:
    StreamingConfig config_;
    std::unique_ptr<WebSocketClient> websocket_client_;
    std::unique_ptr<HTTPClient> http_client_;
    std::unique_ptr<ETSIStandardsAnalyzer> standards_analyzer_;
    std::unique_ptr<GovernmentReporter> government_reporter_;
    std::shared_ptr<Thai::ThaiAnalysisEngine> thai_analyzer_;
    
    // Background processing
    std::thread reporting_thread_;
    std::atomic<bool> running_;
    
    // Statistics tracking
    mutable std::mutex stats_mutex_;
    int reports_sent_;
    int violations_detected_;
    double total_compliance_score_;
    int total_analyses_;
    std::vector<ComplianceResult> recent_violations_;
    
    // Queue management
    std::vector<ETIAnalysisReport> pending_reports_;
    mutable std::mutex queue_mutex_;
    
    void reporting_loop();
    void process_pending_reports();
    void update_statistics(const ETIAnalysisReport& report);
    void handle_critical_violation(const ComplianceResult& violation);
};

// Integration with existing etisnoop command-line interface
class ETISnoopCLIIntegration {
public:
    ETISnoopCLIIntegration();
    
    // Command-line argument parsing for StreamDAB features
    void parse_streamdab_arguments(int argc, char* argv[]);
    
    // Configuration file support
    bool load_config_file(const std::string& config_path);
    bool save_config_file(const std::string& config_path);
    
    // Integration with existing output formats
    void enhance_yaml_output_with_streamdab_data(const ETIAnalysisReport& report);
    void enhance_json_output_with_streamdab_data(const ETIAnalysisReport& report);
    
    // New output formats for StreamDAB
    void output_compliance_csv(const std::vector<ETIAnalysisReport>& reports, const std::string& filename);
    void output_government_report(const ETIAnalysisReport& report, const std::string& filename);
    
    // Real-time monitoring mode
    void enable_realtime_monitoring_mode();
    void disable_realtime_monitoring_mode();

private:
    std::unique_ptr<StreamDABIntegrationEngine> integration_engine_;
    StreamingConfig current_config_;
    bool realtime_mode_enabled_;
    
    void print_streamdab_help();
    void print_thai_analysis_help();
    void print_government_reporting_help();
};

// Utility functions for StreamDAB integration
namespace Utils {

// Convert ETSI standard enum to string
std::string etsi_standard_to_string(ETSIStandard standard);

// Convert violation severity to string with color codes
std::string severity_to_string(ViolationSeverity severity, bool with_colors = false);

// Format compliance score for display
std::string format_compliance_score(double score);

// Generate executive summary from analysis results
std::string generate_executive_summary(const ETIAnalysisReport& report);

// Convert analysis report to various output formats
json report_to_json(const ETIAnalysisReport& report);
std::string report_to_yaml(const ETIAnalysisReport& report);
std::string report_to_csv_row(const ETIAnalysisReport& report);

// Government reporting helpers
std::string format_for_nbtc_submission(const ETIAnalysisReport& report);
bool validate_government_api_response(const std::string& response);

// Performance monitoring
std::string format_performance_metrics(const ETIAnalysisReport& report);

} // namespace Utils

} // namespace StreamDAB