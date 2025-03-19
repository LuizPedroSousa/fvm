// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/

use std::fs;
use std::path::Path;

#[tauri::command]
fn get_directory_contents(path: Option<String>) -> Result<Vec<serde_json::Value>, String> {
    let base_path = match path {
        Some(ref p) => Path::new(p),
        None => Path::new("."),
    };

    let mut entries = Vec::new();
    if let Ok(dir) = fs::read_dir(base_path) {
        for entry in dir {
            if let Ok(entry) = entry {
                let metadata = entry.metadata().map_err(|e| e.to_string())?;
                entries.push(serde_json::json!({
                    "name": entry.file_name().to_string_lossy(),
                    "is_dir": metadata.is_dir()
                }));
            }
        }
    }
    Ok(entries)
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_opener::init())
        .invoke_handler(tauri::generate_handler![get_directory_contents])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
