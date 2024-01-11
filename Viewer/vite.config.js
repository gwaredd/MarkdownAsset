import { defineConfig } from 'vite'
import path from 'path'
import react from '@vitejs/plugin-react'
import { viteSingleFile } from "vite-plugin-singlefile"
import dotenv from 'dotenv';

const flavour = process.env.FLAVOUR || 'dark';
dotenv.config({ path: `.env.${flavour.trim()}` });

export default defineConfig({
  plugins: [
    react(),
    viteSingleFile()
  ],
  optimizeDeps: {
  },
  resolve: {
    alias: [
        {
          find: 'App.css',
          replacement: path.resolve( __dirname, `./src/${process.env.VITE_CSS_PATH}` ),
        },
        {
          find: 'Highlight.css',
          replacement: `highlight.js/styles/${process.env.VITE_HIGHLIGHT_THEME}.css`
        },
    ],
  }
})
